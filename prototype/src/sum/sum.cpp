# include <string>
# include <iostream>
# include <unordered_map>
# include "sum.hpp"


// RegisterModule
RegisterModule(Sum);

// Constructor
Sum::Sum() :
_step(0), _support(0), _global_support(0), _num_trees(0) {
}

// Destructor
Sum::~Sum() {
}

// Process
void Sum::Process() {

	switch (_step) {

		case 0:
			if (_sum()) ++_step;
			else return;
		case 1:
			if (_global()) ++_step;
			else _step = -1;
		default:
			FinishProcess();
			break;

	}

}

// Calculates global frequencies
const bool Sum::_sum() {

	uint size(0);
	switch (Signal()) {

		case 0:
			break;
		case 1:
			Unpack(size, _support);
			_global_support += size;
			return (++_num_trees == Length("TreeData"));

	}

	// Global frequencies
	uint label(0), frequency(0);
	while (Unpack(label, frequency)) {
		if (_table.size() <= label) _table.resize(label + 1, 0);
		_table[label] += frequency;
	}

	return false;

}

// Sends global frequencies
const bool Sum::_global() {

	_support *= _global_support;
	if (_table.empty()) return false;

	for (uint i(_table.size() - 1); i > 0; --i) {
		const uint& freq(_table[i]);
		if (freq and freq >= _support) Pack("Tree", i, freq);
	}

	Signal("Tree", 1, _global_support);
	_table.clear();

	return true;

}
