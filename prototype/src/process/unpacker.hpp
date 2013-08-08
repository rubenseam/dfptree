# include <tuple>
# include <string>
# include <vector>
# include <utility>
# include "__serialize__.hpp"


class Unpacker : public __Serialize__ {

	typedef __Serialize__::S S;

public:

	// Constructor
	Unpacker(const S& max_size) :
	__Serialize__(max_size) {
	}

	// Destructor
	~Unpacker() {
	}

	// Loads data into the buffer
	inline void load(const char *data, const S& size) {
		_load(data, size);
	}

	// Unpacks built-in data types
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   const bool>::type unpack(T& data) {
		return _read(data);
	}

	// Unpacks an array of a built-in data type
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   const bool>::type unpack(T *& data) {

		// Retrieving T[] size
		S size(0);
		if (!_check(size)) return false;

		// Freeing old data
		if (data != NULL) delete[] data;
		size /= _size(T());
		data = new T[size];

		// Reading T[]
		for (S i(0); i < size; ++i) this->_read(data[i]);
		return true;

	}

	// Unpacks a std::string
	inline const bool unpack(std::string& data) {

		// Retrieving std::string length
		S size(0);
		if (!_check(size)) return false;

		// Reading std::string
		data.resize(size);
		for (S i(0); i < size; ++i) {
			this->_read(data[i]);
		}
		return true;

	}

	// Unpacks a std::vector<class T>
	template <class T>
	inline const bool unpack(std::vector<T>& data) {

		// Retrieving std::vector<class T> size
		S size(0);
		if (!_check(size)) return false;

		// Reading std::vector<class T>
		unpack(size);
		data.clear();
		data.reserve(size);

		T entry;
		for (S i(0); i < size; ++i) {
			unpack(entry);
			data.push_back(entry);
		}
		return true;

	}

	// Unpacks a std::pair<class T1, class T2>
	template <class T1, class T2>
	inline const bool unpack(std::pair<T1, T2>& data) {

		// Retrieving std::pair<class T1, class T2> size
		S size(0);
		if (!_check(size)) return false;

		// Reading pair<class T1, class T2>
		unpack(data.first);
		unpack(data.second);
		return true;

	}

	// Unpacks a std::tuple<class... T>
	template <S i = 0, class... T>
	inline typename std::enable_if<(i == 0 && (sizeof...(T) != 0)),
		   const bool>::type unpack(std::tuple<T...>& data) {

		// Retrieving std::tuple<class... T>
		S size(0);
		if (!_check(size)) return false;

		// Reading std::tuple<class... T>
		unpack(std::get<i>(data));
		return unpack<i + 1, T...>(data);

	}

	template <S i = 0, class... T>
	inline typename std::enable_if<(i != 0 && (i < sizeof...(T))),
		   const bool>::type unpack(std::tuple<T...>& data) {
		unpack(std::get<i>(data));
		return unpack<i + 1, T...>(data);
	}

	template <S i = 0, class... T>
	inline typename std::enable_if<(i != 0 and (i == sizeof...(T))),
		   const bool>::type unpack(std::tuple<T...>& data) {
		return true;
	}

	// Unpacks a series of types
	template <bool first = true, class... T>
	inline typename std::enable_if<(first and (sizeof...(T) > 1)),
		   const bool>::type unpack(T&... data) {

		// Retrieving class T...
		S size(0);
		if (!_check(size)) return false;

		// Reading class T...
		return unpack<false>(data...);

	}

	template <bool first, class H, class... T>
	inline typename std::enable_if<!first, const bool>::type
		unpack(H& head, T&... data) {
		unpack(head);
		return unpack<false>(data...);
	}

	template <bool first, class... T>
	inline typename std::enable_if<!first, const bool>::type
		unpack(T&... data) {
		return true;
	}

private:

	// Checks if the buffer has the the given size to be read
	inline const bool _check(S& size) {

		if (!this->_read(size)) {
			return false;
		} else if (!this->_peek(size)) {
			this->_rewind(this->_size());
			return false;
		}
		return true;

	}

};
