# include <string>
# include <fstream>
# include <utility>
# include <iostream>
# include <algorithm>
# include <stdexcept>
# include "tree.hpp"
# include "../utility/reader.hpp"

# define DELIM ';'


// RegisterModule
RegisterModule(Tree);

// Constructor
Tree::Tree() :
__Process__(true),
_step(0), _num_sums(0), _support(0), _global_frequency(0),
_level(0), _reduction_level(0), _fptree(0) {
}

// Destructor
Tree::~Tree() {
	_table.clear();
}

// Process
void Tree::Process() {

	switch (_step) {

		case 0:
			if (_initialize()) _step++;
			else return;
		case 1:
			if (_count()) _step++;
			else throw std::runtime_error("read error");
		case 2:
			if (_sum()) _step++;
			else return;
		case 3:
			if (_build()) _step++;
			else throw std::runtime_error("build error");
		case 4:
			if (_subtrees()) _step++;
			else throw std::runtime_error("subtree error");
		default:
			_fptree.free();
			FinishProcess();

	}

}

// Initializes parameters
const bool Tree::_initialize() {

	return Unpack(_inpath, _support, _level,
			_reduction_level, _outpath);

}

// Reads input and count ocurrences
const bool Tree::_count() {

	Reader<DELIM> reader(_inpath, _table);
	if (!reader.count()) return false;

	for (unsigned int i(_table.size() - 1); i > 0; --i) {
		unsigned int frequency(_table[i]);
		if (frequency) Pack(i, "Sum", i, frequency);
	}

	Signal("Sum", 1, reader.linecount(), _support);
	_table.clear();
	return true;

}

// Receives global frequencies
const bool Tree::_sum() {

	switch (Signal()) {
		case 0:
			break;
		case 1:
			Unpack(_global_frequency);
			return (++_num_sums == Length("SumData"));
	}

	// Global frequencies
	unsigned int label(0), frequency(0);
	while (Unpack(label, frequency)) {
		if (_table.size() <= label) _table.resize(label + 1, 0);
		_table[label] = frequency;
	}

	return false;

}

// Reads the input and builds ordered FPTrees
const bool Tree::_build() {

	_support *= _global_frequency;
	std::vector<unsigned int> data;

	Reader<DELIM> reader(_inpath, _support, _table);
	while (reader.read(data)) _fptree.insert(data);

	std::cout << ">>> " << GetModuleName() << '[' << GetRank()
		<< "] - # nodes: " << _fptree.nodecount() << std::endl;
	// MemInfo();
	return true;

}

// Sends local subtrees
const bool Tree::_subtrees() {

	_fptree.subtrees(DataPacker(this, "Merge"), _level);
	Signal("Merge", 1, _reduction_level, _support,
			_global_frequency, _outpath);
	return true;

}
