# include <unordered_map>
# include "../process/__process__.hpp"


class Sum : public __Process__ {

public:

	// Constructor
	Sum();

	// Destructor
	virtual ~Sum();

	// Process
	void Process();

private:

	int _step;

	float _support;
	uint _global_support;

	uint _num_trees;
	std::vector<uint> _table;

	// Calculates global frequencies
	const bool _sum();

	// Sends global frequencies
	const bool _global();

};
