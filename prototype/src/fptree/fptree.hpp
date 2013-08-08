# include <vector>
# include <iostream>
# include "node.hpp"
# include "__fptree__.hpp"


template <typename T>
class FPTree : public __FPTree__<Node, T> {

public:

	// Constructor
	FPTree(const T& label = T()) :
	__FPTree__<Node, T>(label) {
	}

	// Destructor
	~FPTree() {
	}

	// Inserts a transaction in the FPTree
	inline void insert(const std::vector<T>& transaction) {

		bool allocated(false);
		Node<T> *node(&_root);

		for (uint i(0); i < transaction.size(); ++i) {
			node = node->insert(transaction[i], allocated);
			_num_nodes += allocated;
		}

		node->add_leaf();
		_root.add_frequency();

	}

	// Extracts subtrees
	template <class F>
	inline void subtrees(F&& Pack, const uint& level) {

		T label;
		uint id(0), freq(0);
		std::vector<T> path;

		Node<T> *node;
		typename Node<T>::iterator it(&_root);
		while (node = *it, node != NULL) {

			if ((node->leafcount() and it.level() < level)
					or (it.level() == level)) {

				// Prefix
				path.clear(), id = node->recover(path);
				freq = (it.level() < level) ?
					node->leafcount() : node->frequency();
				Pack(id, char(0), freq, path);

			} else if (it.level() > level) {

				label = node->label(), freq = node->frequency();
				if (!it.changed()) Pack(id, char(1), freq, label);
				else Pack(id, char(2), freq, label, it.level());

			}

			++it;

		}

	}

	// Frees the entire memory pool
	using __FPTree__<Node, T>::free;

	// Number of nodes in the tree
	using __FPTree__<Node, T>::nodecount;

private:

	using __FPTree__<Node, T>::_root;
	using __FPTree__<Node, T>::_num_nodes;

};
