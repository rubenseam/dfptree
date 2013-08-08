# include <tuple>
# include <string>
# include <vector>
# include <utility>
# include "__serialize__.hpp"


class Packer : public __Serialize__ {

	typedef __Serialize__::S S;

public:

	// Constructor
	Packer(const std::string& receiver, const unsigned int& label,
			const int& signal, const S& max_size) :
	__Serialize__(receiver, label, signal, max_size) {
	}

	// Destructor
	~Packer() {
	}

	// Flushes the data from current buffer
	inline void flush() {
		_flush();
	}

	// Flushes the data from current buffer
	template <class... T>
	inline typename std::enable_if<sizeof...(T),
		   void>::type signal(const int& value, const T&... data) {
		_signal(value, data...);
		pack(data...);
		_reset();
	}

	template <class... T>
	inline typename std::enable_if<!sizeof...(T),
		   void>::type signal(const int& value, const T&... data) {
		_signal(value, data...);
		_reset();
	}

	// Packs built-in data types
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   void>::type pack(const T& data) {
		_write(data);
	}

	// Packs an array of a built-in data type
	template <typename T>
	inline typename std::enable_if<std::is_fundamental<T>::value,
		   void>::type pack(const T *& data, const S& length) {

		// Packs the size in bytes
		pack(_size() * length);

		// Packs the data itself
		for (S i(0); i < length; ++i) _write(data[i]);

	}

	// Packs a std::string
	inline void pack(const std::string& data) {

		// Packs the size in bytes
		pack(_size(data));

		// Packs the data itself
		for (S i(0); i < data.length(); ++i) pack(data[i]);

	}

	// Packs a std::vector<class T>
	template <class T>
	inline void pack(const std::vector<T>& data) {

		// Packs the size in bytes
		pack(_size(data) - _size());

		// Packs the data itself
		pack(static_cast<S>(data.size()));
		for (S i(0); i < data.size(); ++i) pack(data[i]);

	}

	// Packs a std::pair<class T1, class T2>
	template <class T1, class T2>
	inline void pack(const std::pair<T1, T2>& data) {

		// Packs the size in bytes
		pack(_size(data) - _size());

		// Packs the data itself
		pack(data.first);
		pack(data.second);

	}

	// Packs a std::tuple<class... T>
	template <S i = 0, class... T>
	inline typename std::enable_if<i == 0, void>::type
		pack(const std::tuple<T...>& data) {

		// Packs the size in bytes
		pack(_size(data) - _size());

		// Packs the data itself
		pack(std::get<i>(data));
		pack<i + 1, T...>(data);

	}

	template <S i = 0, class... T>
	inline typename std::enable_if<(i != 0 and (i < sizeof...(T))),
		   void>::type pack(const std::tuple<T...>& data) {
		pack(std::get<i>(data));
		pack<i + 1, T...>(data);
	}

	template <S i = 0, class... T>
	inline typename std::enable_if<(i != 0 and (i == sizeof...(T))),
		   void>::type pack(const std::tuple<T...>& data) {
	}

	// Packs a series of types
	template <bool first = true, class... T>
	inline typename std::enable_if<(first and (sizeof...(T) > 1)),
		   void>::type pack(const T&... data) {

		// Packs the size in bytes
		pack(_size(data...) - _size());

		// Packs the data itself
		pack<false>(data...);

	}

	template <bool first, class H, class... T>
	inline typename std::enable_if<!first, void>::type
		pack(const H& head, const T&... data) {
		pack(head);
		pack<false>(data...);
	}

	template <bool first, class... T>
	inline typename std::enable_if<!first and (sizeof...(T) == 0),
		   void>::type pack(const T&... data) {
	}

};
