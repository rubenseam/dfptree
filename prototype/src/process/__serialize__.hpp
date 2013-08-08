# include <tuple>
# include <string>
# include <vector>
# include <utility>
# include <stdexcept>

# ifndef __SERIALIZE___HPP
# define __SERIALIZE___HPP


class __Serialize__ {

public:

	typedef unsigned int S;
	std::vector<std::vector<char> > messages;

	// Constructor -- packer
	__Serialize__(const std::string& name, const unsigned int& label,
			const int& signal, const S& max_size) :
	_h(0),
	_offset(_sizeof(name) + 1 + _sizeof(label) + _sizeof(signal)),
	_min_size(_offset + 8),
	_max_size((max_size > _min_size) ? max_size : _min_size) {
		_header(name, label, signal);
	}

	// Constructor -- unpacker
	__Serialize__(const S& max_size) :
	_h(0), _offset(0), _min_size(8),
	_max_size((max_size > _min_size) ? max_size : _min_size) {
	}

	// Destructor
	~__Serialize__() {
		_buffer.clear();
		messages.clear();
	}

protected:

	// Retrieves the size, in bytes, of S
	template <class... T>
	inline typename std::enable_if<!sizeof...(T), const S>::type
	_size() {
		return _sizeof();
	}

	// Retrieves the size, in bytes, of T... data
	template <class... T>
	inline typename std::enable_if<sizeof...(T), const S>::type
	_size(const T&... data) {
		return _sizeof(data...);
	}

	// Checks the size available in the buffer
	inline const bool _peek(const S& size) const {
		return !(_h + size > _buffer.size());
	}

	// Move back the buffer head by "size" bytes
	inline void _rewind(const S& size) {
		_h -= size;
	}

	// Loads data into the buffer
	inline void _load(const char *& data, const S& size) {
		_buffer.insert(_buffer.end(), &data[0], &data[size]);
	}

	// Reads a built-in type from the buffer
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   const bool>::type _read(T& data) {

		if (_h > _max_size) {
			_buffer.erase(_buffer.begin(), _buffer.begin() + _h);
			_h = 0;
		}

		if (_h + _sizeof(data) > _buffer.size()) return false;
		data = *(reinterpret_cast<T *>(&_buffer[_h]));
		_h += _sizeof(data);
		return true;

	}

	// Flushes data from buffer
	inline void _flush(const bool& force = false) {

		if (!force and _buffer.size() == _offset) return;
		messages.push_back(_buffer);
		_buffer.erase(_buffer.begin() + _offset, _buffer.end());

	}

	// Writes data into buffer
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   void>::type _write(const T& data) {

		if (_buffer.size() + _sizeof(data) > _max_size) _flush();
		_buffer.insert(_buffer.end(),
				reinterpret_cast<const char *>(&data),
				reinterpret_cast<const char *>(&data) +
				_sizeof(data));

	}

	// Writes the given signal value into the buffer
	template <class... T>
	inline void _signal(const int& value, const T&... data) {

		if (_sizeof(data...) >= _max_size) {
			std::string error("data size exceeds message capacity");
			throw std::range_error(error);
		}

		// Changes signal value
		_flush();
		_buffer.erase(_buffer.begin() + _offset - _sizeof(value),
				_buffer.end());
		_write(value);

	}

	// Resets the message header
	inline void _reset() {
		_flush(true);
		_buffer = _header_backup;
	}

private:

	S _h, _offset;
	S _min_size, _max_size;

	std::vector<char> _buffer;
	std::vector<char> _header_backup;

	// Writes a header into the buffer
	inline void _header(const std::string& name,
			const unsigned int& id, const int& signal) {

		_write(id);

		_buffer.insert(_buffer.end(), name.begin(), name.end());
		_buffer.push_back('\0');

		_write(signal);
		_header_backup = _buffer;

	}

	// Retrieves the size, in bytes, of S
	inline const S _sizeof() const {
		return sizeof(S);
	}

	// Retrieves the size, in bytes, of a built-in type
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   const S>::type _sizeof(const T& data) const {
		return sizeof(T);
	}

	// Retrieves the size, in bytes, of a std::string
	inline const S _sizeof(const std::string& data) const {
		return data.length();
	}

	// Retrieves the size, in bytes, of a std::vector<class T>
	template <class T>
	inline const S _sizeof(const std::vector<T>& data) const {

		S size(2 * _sizeof());
		for (unsigned int i(0); i < data.size(); ++i) {
			size += _sizeof(data[i]);
		}
		return size;

	}

	// Retrieves the size, in bytes, of a std::pair<T1, T2>
	template <class T1, class T2>
	inline const S _sizeof(const std::pair<T1, T2>& data) {
		return _sizeof() + _sizeof(data.first) +
			_sizeof(data.second);
	}

	// Retrieves the size, in bytes, of a std::tuple<class... T>
	template <unsigned int i = 0, class... T>
	inline typename std::enable_if<i == sizeof...(T), const S>::type
	_sizeof(const std::tuple<T...>& data, const S size) {
		return size + _sizeof();
	}

	template <unsigned int i = 0, class... T>
	inline typename std::enable_if<i < sizeof...(T), const S>::type
	_sizeof(const std::tuple<T...>& data, const S size = 0) {
		return _sizeof<i + 1, T...>(data,
				size + _sizeof(std::get<i>(data)));
	}

	// Retrieves the size, in bytes, of a series of types
	template <class H, class... T>
	inline typename std::enable_if<sizeof...(T) == 1, const S>::type
	_sizeof(const H& head, const T&... data) {
		return _sizeof(head) + _sizeof(data...) + _sizeof();
	}

	template <class H, class... T>
	inline typename std::enable_if<(sizeof...(T) > 1), const S>::type
	_sizeof(const H& head, const T&... data) {
		return _sizeof(head) + _sizeof(data...);
	}

};


# endif
