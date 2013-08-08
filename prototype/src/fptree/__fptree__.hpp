# include <vector>
# include <iostream>


template <template <typename> class Node, typename T>
class __FPTree__ {

protected:

	Node<T> _root;
	size_t _num_nodes;

	// Constructor
	__FPTree__(const T& label = T()) :
	_root(label), _num_nodes(0) {
	}

	// Destructor
	~__FPTree__() {

		std::vector<Node<T> *> path;
		typename Node<T>::iterator it(&_root);

		while (*it != NULL) {
			path.push_back(*it), ++it;
			while (it.level() < path.size()) {
				delete path.back(), path.pop_back();
			}
		}

		_root.nullify();

	}

	// Frees the entire memory pool
	inline static void static_free() {
		Node<T>::free();
	}

	// Frees the entire memory pool
	inline void free() {
		Node<T>::free();
		_root.nullify();
	}

	// Number of nodes in the tree
	inline const size_t nodecount() const {
		return _num_nodes;
	}

	// Output stream
	inline friend std::ostream& operator<<(std::ostream& outstream,
			__FPTree__<Node, T>& fptree) {

		outstream << "LEVEL;LABEL;FREQ\n0" << fptree._root << '\n';
		typename Node<T>::iterator it(&fptree._root);

		while (*it != NULL) {
			outstream << it.level() << *(*it) << "\n";
			++it;
		}

		return outstream;

	}

};
