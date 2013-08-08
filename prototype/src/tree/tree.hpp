# include <string>
# include <vector>
# include <utility>
# include "../process/__process__.hpp"
# include "../fptree/fptree.hpp"


class Tree : public __Process__ {

public:

	// Constructor
	Tree();

	// Destructor
	virtual ~Tree();

	// Process
	void Process();

private:

	int _step;
	uint _num_sums;

	float _support;
	uint _global_frequency;

	uint _level, _reduction_level;
	std::string _inpath, _outpath;

	FPTree<uint> _fptree;
	std::vector<uint> _table;

	// Initializes parameters
	const bool _initialize();

	// Reads input and count ocurrences
	const bool _count();

	// Receives global frequencies
	const bool _sum();

	// Reads the input and builds ordered FPTrees
	const bool _build();

	// Sends local subtrees
	const bool _subtrees();

};
