# include <vector>
# include "__node__.hpp"


template <typename T>
class Node : public __Node__<Node, T> {

public:

	// Constructor
	Node(const T& label = T(), Node<T> *parent = NULL) :
	__Node__< ::Node, T>(label, parent) {
	}

	// Destructor
	~Node() {
	}

};

