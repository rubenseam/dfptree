# include <string>
# include <fstream>
# include <sstream>
# include <iostream>
# include <stdexcept>

# ifndef READER_HPP
# define READER_HPP

# define MAX_LENGTH 8192


class __Reader__ {

public:

	// Constructor
	__Reader__(const std::string& inpath) :
	_pos(0), _num_items(0), _num_lines(0),
	_inpath(inpath), _infile(inpath, std::ios::in),
	_h(0), _t(0), _buffer(MAX_LENGTH, '\0') {
		std::string error("invalid file " + inpath);
		if (!_read()) throw std::logic_error(error);
	}

	// Destructor
	~__Reader__() {
		_infile.close();
		_buffer.clear();
	}

	// Number of items
	inline const uint& itemcount() const {
		return _num_items;
	}

	// Number of lines
	inline const uint& linecount() const {
		return _num_lines;
	}

protected:

	uint _pos;
	uint _num_items, _num_lines;

	// Next char in buffer
	inline const bool _next(char& c) {

		if (!_read()) return false;

		c = _buffer[_h];
		++_pos, ++_h;

		if (c == '\n') _pos = 1, ++_num_lines;
		return true;

	}

	// Error message
	inline void _error(const char& c) {

		std::stringstream ss;
		ss << _inpath << ":" << _num_lines + 1 << ":"
			<< _pos << " character '" << c << "' found";
		throw std::logic_error(ss.str());

	}

private:

	std::string _inpath;
	std::ifstream _infile;

	uint _h, _t;
	std::vector<char> _buffer;

	// Reads data from infile
	inline const bool _read() {

		if (_h < _t) return true;
		if (!_infile) return false;

		_h = 0;
		_infile.read(&_buffer[0], _buffer.size());
		_t = _infile.gcount();
		return (_h < _t);

	}

};


template <char delim = ' '>
class Reader : public __Reader__ {

public:

	// Constructor -- counter
	Reader(const std::string& inpath,
			std::vector<uint>& table) :
	__Reader__(inpath), _valid(false), _item(0),
	_support(0), _table(table) {
	}

	// Constructor -- filter
	Reader(const std::string& inpath, const float& support,
			std::vector<uint>& table) :
	__Reader__(inpath), _valid(false), _item(0),
	_support(support), _table(table) {
	}

	// Destructor
	~Reader() {
	}

	// Calculates frequencies
	inline const bool count() {

		char c('\0');
		if (!_next(c)) return false;

		do switch (c) {

			case delim: case '\n':

				if (!_valid) break;
				if (_table.size() <= _item) {
					_table.resize(_item + 1, 0);
				}
				++_table[_item];

				++_num_items, _item = 0, _valid = false;
				break;

			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				_item = 10 * _item + c - '0', _valid = true;
				break;

			default:
				_error(c);

		} while (_next(c));

		return true;

	}

	// Reads a sequence of unsigned int
	inline const bool read(std::vector<uint>& data) {

		char c('\0');
		data.clear();
		if (!_next(c)) return false;

		do switch (c) {

			case delim: case '\n':

				if (!_valid) break;
				if (_item < _table.size() and _table[_item]) {
					data.push_back(_item);
					++_num_items;
				}

				_item = 0, _valid = false;
				if (c == delim) break;

				std::sort(data.begin(), data.end(), _sort(_table));
				return true;

			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				_item = 10 * _item + c - '0', _valid = true;
				break;

			default:
				_error(c);

		} while (_next(c));

		std::sort(data.begin(), data.end(), _sort(_table));
		return true;

	}

private:

	bool _valid;
	unsigned _item;

	const float _support;
	std::vector<uint>& _table;

	struct _sort {

	public:

		// Constructor
		_sort(const std::vector<uint>& table) :
		_table(table) {
		}

		// operator()
		inline const bool operator()(const uint& a, const uint& b) {
			const uint& f_a(_table[a]);
			const uint& f_b(_table[b]);
			return (f_a > f_b or (f_a == f_b and a < b));
		}

	private:

		const std::vector<uint>& _table;

	};

};


# endif
