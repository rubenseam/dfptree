# include <vector>
# include <utility>
# include <stdexcept>
# include <unordered_set>

# ifndef MEMORY_HPP
# define MEMORY_HPP
 
# define MAX_SHIFT 12
# define BASE_SIZE 1024


template <class T>
class Memory {

protected:

	// Constructor
	Memory() {
		std::string error("class smaller than a pointer");
		if (!_base or sizeof(T) < sizeof(T *)) {
			throw std::logic_error(error);
		}
	}

	// Destructor
	~Memory() {
	}

	// Allocates memory
	inline static void * _allocate() {
		if (_freed == NULL) _increase();
		return _allocate_block();
	}

	// Frees memory
	inline static void _free(void *& address) {
		_free_block(static_cast<T *>(address));
	}

	// Frees the entire memory pool
	inline static void free() {
		for (uint i(0); i < _memory.size(); ++i) delete _memory[i];
		_memory.clear();
	}

private:

	static T *_freed;
	static size_t _free_count;
	static std::vector<T *> _memory;

	static size_t _size, _base;
	static size_t _shift, _max;

	// Increases the memory available
	inline static void _increase() {

		_size = _base << _shift;
		if (_shift < _max) _shift += 4;

		_freed = static_cast<T *>(::operator new(sizeof(T[_size])));
		if (_size > 1) _link(_freed, &_freed[1]);

		_memory.push_back(_freed);

	}

	// Allocates a memory block
	inline static void * _allocate_block() {

		T *address(_freed);
		_unlink(_freed);

		if (_free_count) --_free_count;
		else if (_size > 1) {
			--_size;
			_link(_freed, &_freed[1]);
		} else _freed = NULL;

		return address;

	}

	// Frees a memory block
	inline static void _free_block(T *address) {
		_free_count++;
		_link(address, _freed);
		_freed = address;
	}

	// Registers a pointer in the given address
	inline static void _link(T *& address, T *pointer) {
		*(reinterpret_cast<T **>(address)) = pointer;
	}
	inline static void _link(void *& address, T *pointer) {
		*(static_cast<T **>(address)) = pointer;
	}

	// Retrieves the content of the given pointer
	inline static void _unlink(T *& pointer) {
		pointer = *(reinterpret_cast<T **>(pointer));
	}

};

// Definitions
template <class T>
T * Memory<T>::_freed(NULL);

template <class T>
size_t Memory<T>::_free_count(0);

template <class T>
std::vector<T *> Memory<T>::_memory;

template <class T>
size_t Memory<T>::_size(0);

template <class T>
size_t Memory<T>::_base(BASE_SIZE);

template <class T>
size_t Memory<T>::_shift(0);

template <class T>
size_t Memory<T>::_max(MAX_SHIFT);


# endif
