# include <set>
# include <vector>
# include <fstream>
# include <unordered_map>
# include "../process/__process__.hpp"
# include "../fptree/cfptree.hpp"


class Reduce : public __Process__ {

public:

	// Constructor
	Reduce();

	// Destructor
	virtual ~Reduce();

	// Process
	void Process();

private:

	int _step;
	unsigned int _num_merges;

	float _support;
	unsigned int _global_frequency;

	std::string _outpath;
	std::ofstream _outfile;

	unsigned int _level;
	std::set<unsigned int> _ordered;
	std::vector<unsigned int> _label;
	std::unordered_map<unsigned int, CFPTree<unsigned int> *> _tree;

	// Initializes the vector of labels
	const bool _init();

	// Reduces local CFPTrees received
	const bool _reduce();

	// Checks the reduction vector for reduced trees
	void _reduced_trees();

	// Reduces the local FPTree
	void _reduction(const unsigned int& source,
			const unsigned int& id);

	// FPGrowth algorithm
	const bool _fpgrowth();

};
