# include <string>
# include <vector>
# include <fstream>
# include <iostream>
# include <stdexcept>
# include "reduce.hpp"


// RegisterModule
RegisterModule(Reduce);

// Constructor
Reduce::Reduce() :
__Process__(true),
_step(0), _num_merges(0), _support(0), _global_frequency(0) {
}

// Destructor
Reduce::~Reduce() {
}

// Process
void Reduce::Process() {

	switch (_step) {
	case 0:
		if (_init()) ++_step;
		else throw std::runtime_error("initialization error");
	case 1:
		if (_reduce()) ++_step;
		else return;
	default:
		std::cout << "Reduce[" << GetRank()
			<< "]: execution " << (GetClockTime() - 5)
			<< "s;" << std::endl;
		CFPTree<uint>::static_free();
		FinishProcess();
	}

}

// Initializes the vector of labels
const bool Reduce::_init() {

	_label.resize(Length("MergeData"), 0);
	return true;

}

// Reduces local CFPTrees received
const bool Reduce::_reduce() {

	std::stringstream ss;
	uint label(0), frequency(0);
	uint source(Source()), id(_label[source]);
	switch (Signal()) {

	case 0:
		_reduction(source, id);
		return false;

	case 1:
		Unpack(_support, _global_frequency, _outpath);
		if (!_outfile.is_open()) {
			ss << _outpath << GetRank() << ".txt";
			_outpath = ss.str();
			_outfile.open(_outpath, std::ios::out);
		}
		return false;

	case 2:
		Unpack(label, frequency);
		id = label, _label[source] = label;
		if (_tree.find(id) == _tree.end()) {
			_tree[id] = new CFPTree<uint>();
			_tree[id]->init(Length("MergeData"));
			_ordered.insert(id);
		}
		_tree[id]->init(label, frequency), _reduced_trees();
		return false;

	case 3:
		_label[source] = -1, _reduced_trees();
		return (++_num_merges == Length("MergeData"));

	}

}

// Reduces the local FPTree
void Reduce::_reduction(const uint& source, const uint& id) {

	bool new_path(false);
	uint label(0), frequency(0);
	while (Unpack(label, frequency, new_path)) {
		uint level(0); if (new_path) Unpack(level);
		_tree[id]->merge(source, label, frequency, level);
	}

}

// Checks the reduction vector for reduced trees
void Reduce::_reduced_trees() {

	uint smallest(-1);
	for (uint i(0); i < _label.size(); ++i) {
		if (_label[i] < smallest) smallest = _label[i];
	}

	auto it(_ordered.begin());
	while (it != _ordered.end() and *it < smallest) {
		const auto& id(*it);
		_tree[id]->fpgrowth(_support, _outfile);
		delete _tree[id], _tree[id] = NULL, ++it;
	}

	_ordered.erase(_ordered.begin(), it);

}
