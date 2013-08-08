# include "../process/__process__.hpp"


class Init : public __Process__ {

public:

	// Constructor
	Init();

	// Destructor
	virtual ~Init();

	// Process
	void Process();

private:

	bool _indexed;
	std::string _inpath;
	std::string _outpath;

	float _support;
	uint _level, _reduction_level;

};
