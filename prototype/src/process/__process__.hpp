# include <map>
# include <string>
# include <fstream>
# include <stdexcept>
# include "__message__.hpp"

# define TIMEOUT 1e5


class __Process__ : public __Message__ {

protected:

	// Constructor
	__Process__(const bool& info = false) :
	__Message__(), _info(info),
	_configured(false), _has_finished(false) {
	}

	// Destructor
	virtual ~__Process__() {
		if (_info) Info("sent"), Info("received");
	}

	// Processes the message received
	virtual void Process() = 0;

	// Processes the message received
	void Process(Message& message) {

		try {

			if (!_configured) _setup();
			if (_has_finished) usleep(TIMEOUT);
			else if (Read(message) or Signal()) Process();

		} catch (const std::exception& error) {

			time_t rawtime;
			struct tm *timeinfo;
			char str[100] = {'\0'};

			time(&rawtime);
			timeinfo = localtime(&rawtime);
			strftime(str, 100, "[%Y-%m-%d %H:%M:%S]", timeinfo);

			std::cout << str << " - " << GetModuleName()
				<< "[" << GetRank() << "]:\n    Exception: "
				<< error.what() << "\n    Exiting...\n\n"
				<< std::endl;
			exit(1);

		}

	}

	// Displays memory status
	inline void MemInfo() {

		std::ifstream statm("/proc/self/statm");
		size_t mem_vrt, mem_res, mem_shd;

		statm >> mem_vrt >> mem_res >> mem_shd;
		std::cout << ">>> " << GetModuleName() << '[' << GetRank()
			<< "] - Memory (v, r, s): " << mem_vrt << ' '
			<< mem_res << ' ' << mem_shd << std::endl;
		statm.close();

	}

	// Finishes the entire process
	inline void FinishProcess() {
		_has_finished = true;
		// TerminateModule();
	}

private:

	bool _info;
	bool _configured;
	bool _has_finished;

	// Sets up consumers and producers
	inline void _setup() {

		_configured = true;
		std::map<std::string, unsigned int> consumers, producers;

		GetGrid(consumers, producers);
		Config(GetRank(), GetModuleName(), consumers, producers);

	}

};
