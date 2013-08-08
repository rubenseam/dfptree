# include <string>
# include <vector>
# include <iostream>
# include <stdexcept>
# include "merge.hpp"


// RegisterModule
RegisterModule(Merge);

// Constructor
Merge::Merge() :
__Process__(true),
_step(0), _num_trees(0), _support(0),
_level(0), _global_frequency(0) {
}

// Destructor
Merge::~Merge() {
}

// Process
void Merge::Process() {

	switch (_step) {
	case 0:
		if (_init()) ++_step;
		else throw std::runtime_error("initialization error");
	case 1:
		if (_merge()) ++_step;
		else return;
	case 2:
		if (_projections()) ++_step;
		else throw std::runtime_error("projection error");
	default:
		_cfptree.free();
		FinishProcess();
	}

}

// Initializes root nodes
const bool Merge::_init() {

	_cfptree.init(Length("TreeData"));
	return true;

}

// Merges local subtrees received
const bool Merge::_merge() {

	switch (Signal()) {
	case 0:
		break;
	case 1:
		Unpack(_level, _support, _global_frequency, _outpath);
		return (++_num_trees == Length("TreeData"));
	}

	// Local FPTrees
	char type(0);
	std::vector<uint> path;
	uint level(0), label(0), frequency(0);

	const uint source(Source());
	while (Unpack(type, frequency)) {

		switch (type) {
		case 0:
			path.clear(), Unpack(path);
			_cfptree.merge(source, path.front(), frequency, 1);
			for (uint i(1); i < path.size(); ++i) {
				_cfptree.merge(source, path[i], frequency);
			}
			break;
		case 1:
			Unpack(label);
			_cfptree.merge(source, label, frequency);
			break;
		case 2:
			Unpack(label), Unpack(level);
			_cfptree.merge(source, label, frequency, level);
			break;
		}

	}

	path.clear();
	return false;

}

// Sends local FPTree projections
const bool Merge::_projections() {

	std::cout << ">>> " << GetModuleName() << '[' << GetRank()
		<< "] - # nodes: " << _cfptree.nodecount() << std::endl;
	// MemInfo();

	Signal("Reduce", 1, _support, _global_frequency, _outpath);
	_cfptree.project(DataPacker(this, "Reduce"),
			DataSignaler(this, "Reduce", 2), _level);

	Signal("Reduce", 3);
	return true;

}
