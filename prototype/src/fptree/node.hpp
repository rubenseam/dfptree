# include "__node__.hpp"


template <typename T>
class Node : public __Node__<Node, T> {

public:

	// Constructor
	Node(const T& label = T(), Node<T> *parent = NULL) :
	__Node__< ::Node, T>(label, parent), _leafcount(0) {
	}

	// Destructor
	~Node() {
	}

	// Leaf count
	inline const uint& leafcount() const {
		return _leafcount;
	}

	// Inserts a leaf node
	inline void add_leaf() {
		++_leafcount;
	}

private:

	uint _leafcount;

};
