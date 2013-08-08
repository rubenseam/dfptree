# include <string>
# include <iomanip>
# include "init.hpp"


// RegisterModule
RegisterModule(Init);

// Constructor
Init::Init() {

	_inpath = GetArgument("inpath");
	_outpath = GetArgument("outpath");
	_support = atoi(GetArgument("support").c_str()) / float(100);
	_level = atoi(GetArgument("level").c_str());
	_indexed = atoi(GetArgument("indexed").c_str());
	_reduction_level = atoi(GetArgument("reduction_level").c_str());

}

// Destructor
Init::~Init() {
}

// Process
void Init::Process() {

	std::stringstream ss;
	for (uint i(0); i < Length("Tree"); ++i) {

		ss.str(""), ss << _inpath;
		ss << std::setfill('0') << std::setw(2);

		if (_indexed) ss << i;
		Pack(i, "Tree", ss.str(), _support, _level,
				_reduction_level, _outpath);

	}

	Flush("Tree");
	FinishProcess();

}
