# include "../process/__process__.hpp"
# include "../fptree/cfptree.hpp"


class Merge : public __Process__ {

public:

	// Constructor
	Merge();

	// Destructor
	virtual ~Merge();

	// Process
	void Process();

private:

	int _step;
	uint _num_trees;

	float _support;
	uint _global_frequency;
	std::string _outpath;

	uint _level;
	CFPTree<uint> _cfptree;

	// Initializes root nodes
	const bool _init();

	// Merges local subtrees received
	const bool _merge();

	// Sends local FPTree projections
	const bool _projections();

};
