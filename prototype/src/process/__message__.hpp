# include <map>
# include <ctime>
# include <string>
# include <vector>
# include <iomanip>
# include <iostream>
# include <snappy.h>
# include <watershed.h>
# include "packer.hpp"
# include "unpacker.hpp"

# define MIN_MSGS (1 << 4)
# define MAX_SIZE (1 << 13)


class __Message__ : public ProcessingModule {

	typedef __Serialize__::S S;

public:

	// Retrieves the source label
	inline const unsigned int& Source() const {
		return _label;
	}

	// Retrieves the signal value
	inline const int& Signal() const {
		return _signal;
	}

	// Sends a signal to the selected receivers
	template <class... T>
	inline const unsigned int Signal(const std::string& name,
			const int& value, const T&... data) {

		for (unsigned int i(0); i < _packer[name].size(); ++i) {
			_packer[name][i]->signal(value, data...);
			_send(_packer[name][i]);
		}

		return _packer[name].size();

	}

	// Sends a signal to the selected receiver instance
	template <class... T>
	inline const bool Signal(unsigned int label,
			const std::string& name, const int& value,
			const T&... data) {

		if (_packer[name].empty()) return false;

		label %= _packer[name].size();
		_packer[name][label]->signal(value, data...);
		_send(_packer[name][label]);
		return true;

	}

	// Retrieves the number of instances of a consumer/producer
	inline const S Length(const std::string& name) const {

		auto it_p(_packer.find(name));
		if (it_p != _packer.end()) return it_p->second.size();

		auto it_u(_unpacker.find(name));
		if (it_u != _unpacker.end()) return it_u->second.size();

		return 0;

	}

	// Generates a Packer function bound to a specific receiver
	class DataPacker {

	public:

		DataPacker(__Message__* p, const std::string& name) :
		_p(p), _stream(name) {
		}

		~DataPacker() {
		}

		template <class... T>
		inline const bool operator()(const unsigned int& id,
				T&&... data) {
			return _p->Pack(id, _stream, std::forward<T>(data)...);
		}

	private:

		__Message__ *_p;
		const std::string _stream;

	};

	// Generates a Signaler function bound to a specific receiver
	class DataSignaler {

	public:

		DataSignaler(__Message__* p, const std::string& name,
				const unsigned int& value) :
		_p(p), _stream(name), _value(value) {
		}

		~DataSignaler() {
		}

		template <class... T>
		inline const bool operator()(const unsigned int& id,
				T&&... data) {
			return _p->Signal(id, _stream, _value,
					std::forward<T>(data)...);
		}

	private:

		__Message__ *_p;
		const std::string _stream;
		const unsigned int _value;

	};

	// Serializes data to the selected receivers
	template <class... T>
	inline const unsigned int Pack(const std::string& name,
			const T&... data) {

		for (unsigned int i(0); i < _packer[name].size(); ++i) {
			_packer[name][i]->pack(data...);
			_send(_packer[name][i], false);
		}

		return _packer[name].size();

	}

	// Serializes data to the selected receiver instance
	template <class... T>
	inline const bool Pack(unsigned int label,
			const std::string& name, const T&... data) {

		if (_packer[name].empty()) return false;

		label %= _packer[name].size();
		_packer[name][label]->pack(data...);
		_send(_packer[name][label], false);
		return true;

	}

	// Unpacks data from the message received
	template <class... T>
	inline const bool Unpack(T&... data) {
		return _unpacker[_stream][_label]->unpack(data...);
	}

	// Flushes the buffer of the selected receivers
	inline const unsigned int Flush(const std::string& name) {

		for (unsigned int i(0); i < _packer[name].size(); ++i) {
			_packer[name][i]->flush();
			_send(_packer[name][i]);
		}

		return _packer[name].size();

	}

	// Flushes the buffer of the selected receiver instance
	inline const bool Flush(const std::string& name,
			unsigned int label) {

		if (_packer[name].empty()) return false;

		label %= _packer[name].size();
		_packer[name][label]->flush();
		_send(_packer[name][label]);
		return true;

	}

	// Outputs statistics on the selected communication flow
	inline void Info(const std::string& f) const {

		if (f == "sent") {
			_info(f, _num_sent, _min_sent, _max_sent, _total_sent);
		} else if (f == "received") {
			_info(f, _num_recv, _min_recv, _max_recv, _total_recv);
		}

	}

protected:

	// Constructor
	__Message__() :
	_signal(0), _limit(MAX_SIZE), _min_msgs(MIN_MSGS),
	_max_sent(0), _min_sent(_limit), _num_sent(0), _total_sent(0),
	_max_recv(0), _min_recv(_limit), _num_recv(0), _total_recv(0) {

		std::ios_base::sync_with_stdio(false);
		std::cout.precision(3);
		std::cout.setf(std::ios::fixed, std::ios::floatfield);

	}

	// Destructor
	~__Message__() {
	}

	// Sets up consumers and producers
	inline void Config(const unsigned int& id,
			const std::string& name,
			const std::map<std::string, unsigned int>& consumer,
			const std::map<std::string, unsigned int>& producer) {

		_id = id;
		_name = name;

		for (auto it(consumer.begin()); it != consumer.end(); ++it) {
			for (unsigned int i(0); i < it->second; ++i) {
				_packer[it->first].push_back(new Packer(it->first, i,
							_signal, _limit));
			}
		}

		for (auto it(producer.begin()); it != producer.end(); ++it) {
			for (unsigned int i(0); i < it->second; ++i) {
				_unpacker[it->first].push_back(new Unpacker(_limit));
			}

		}

	}

	// Reads the message received
	inline const bool Read(Message& message) {
		return _recv(message);
	}

private:

	unsigned int _id;
	std::string _name;

	int _signal;
	unsigned int _label;
	std::string _stream;

	S _limit, _min_msgs;
	S _max_sent, _min_sent, _num_sent, _total_sent;
	S _max_recv, _min_recv, _num_recv, _total_recv;

	std::map<std::string, std::vector<Packer *> > _packer;
	std::map<std::string, std::vector<Unpacker *> > _unpacker;

	// Registers communication
	inline void _register(Message& message, S& num_messages,
			S& min_size, S& max_size, S& total_size) const {

		size_t size(message.GetDataSize());

		num_messages++;
		total_size += size;
		if (min_size > size) min_size = size;
		if (max_size < size) max_size = size;

	}

	// Sends messages from the given packer
	inline void _send(Packer *packer, const bool& force = true) {

		if (!force and packer->messages.size() < _min_msgs) return;
		for (uint i(0); i < packer->messages.size(); ++i) {

			auto m(packer->messages[i]);
			Message msg(static_cast<void *>(m.data()), m.size());

			_register(msg, _num_sent, _min_sent,
					_max_sent, _total_sent);
			ProcessingModule::Send(msg);

		}

		packer->messages.clear();

	}

	// Reads the message received
	inline const bool _recv(Message& message) {

		S size(message.GetDataSize());
		if (!size) return _unpacker.empty();
		_register(message, _num_recv, _min_recv,
				_max_recv, _total_recv);

		char *data(static_cast<char *>(message.GetData()));

		// Receiver's label
		unsigned int label(
				*(reinterpret_cast<unsigned int *>(data)));
		data += sizeof(label);
		size -= sizeof(label);

		// Receiver's name
		std::string name(reinterpret_cast<char *>(data));
		data += name.size() + 1;
		size -= (name.size() + 1);

		// Signal value
		_signal = *(reinterpret_cast<int *>(data));
		data += sizeof(_signal);
		size -= sizeof(_signal);

		if (_name != name) {
			_signal = 0;
			return false;
		}

		// Reads the message content
		_label = message.GetSource();
		_stream = message.GetSourceStream();
		_unpacker[_stream][_label]->load(data, size);

		return (!_signal);

	}

	// Communication statistics
	inline const std::string _comm(const S& num_messages,
			const S& min_size, const S& max_size,
			const S& total_size) const {

		std::stringstream ss;

		const unsigned int kB(1024);
		ss << std::fixed << std::setw(3) << std::setprecision(3)
			<< "    min size: " << (min_size / float(kB))
			<< "kB (" << min_size << " bytes);\n"
			<< "    max size: " << (max_size / float(kB))
			<< "kB (" << max_size << " bytes);\n"
			<< "    avg size: "
			<< (total_size / float(num_messages * kB))
			<< "kB (" << (total_size / num_messages)
			<< " bytes);\n"
			<< "    total size: ";

		if (total_size <= (kB << 10)) {
			ss << (total_size / float(kB))
				<< "kB (" << total_size << " bytes);";
		} else if (total_size <= (kB << 20)) {
			ss << (total_size / float(kB << 10))
				<< "MB (" << total_size << " bytes);";
		} else {
			ss << (total_size / float(kB << 20))
				<< "GB (" << total_size << " bytes);";
		}

		return ss.str();

	}

	// Prints out the communication statistics
	void _info(const std::string& flow,
			const S& num_messages, const S& min_size,
			const S& max_size, const S& total_size) const {

		time_t rawtime;
		struct tm *timeinfo;
		char str[100] = {'\0'};

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(str, 100, "[%Y-%m-%d %H:%M:%S]", timeinfo);

		std::cout << str << " - " << _name << "[" << _id << "]:\n";
		if (!num_messages) {
			std::cout << "    no messages " << flow
				<< ";" << std::endl;
		} else {
			std::cout << "    number of messages " << flow
				<< ": " << num_messages << ";\n"
				<< _comm(num_messages, min_size,
						max_size, total_size)
				<< std::endl;
		}

	}

};
