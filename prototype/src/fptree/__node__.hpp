# include <iostream>
# include <algorithm>
# include "../utility/memory.hpp"


template <template <typename> class Node, typename T>
class __Node__ : private Memory<Node<T> > {

public:

	// Constructor
	__Node__(const T& label = T(), Node<T> *parent = NULL) :
	_label(label), _frequency(0),
	_child(NULL), _parent(parent), _sibling(NULL) {
	}

	// Destructor
	~__Node__() {
	}

	// new operator
	inline static void * operator new(const size_t size) {
		return Node<T>::_allocate();
	}

	// delete operator
	inline static void operator delete(void *address) {
		Node<T>::_free(address);
	}

	// Frees the entire memory pool
	inline static void free() {
		Memory<Node<T> >::free();
	}

	// Invalidates pointers
	inline void nullify() {
		_child = NULL, _parent = NULL, _sibling = NULL;
	}

	// Iterator
	class iterator;

	// Output stream
	inline friend std::ostream& operator<<(std::ostream& outstream,
			const Node<T>& node) {
		outstream << ';' << node._label << ';' << node._frequency;
		return outstream;
	}

	// Label
	inline const T& label() const {
		return _label;
	}

	// Frequency
	inline const uint& frequency() const {
		return _frequency;
	}

	// Parent
	inline Node<T> * const& parent() const {
		return _parent;
	}

	// Sets the label
	inline void set_label(const T& label) {
		_label = label;
	}

	// States whether the current node holds a single path
	inline const bool single_path() {

		Node<T> *node(reinterpret_cast<Node<T> *>(this));
		while (node != NULL) {
			if (node->_sibling != NULL) return false;
			node = node->_child;
		}

		return true;

	}

	// Adds frequency
	inline void add_frequency(const uint& frequency = 1) {
		_frequency += frequency;
	}

	// Inserts a child node
	inline Node<T> * insert(const T& label, bool& allocated,
			const uint& frequency = 1) {

		Node<T> *node(_getchild(label, allocated));
		node->_frequency += frequency;
		return node;

	}

	// Recovers the current path
	inline const uint recover(std::vector<T>& path,
			const bool& reverse = true) {

		path.clear();
		uint id(0), x(33);

		Node<T> *node(reinterpret_cast<Node<T> *>(this));
		while (node != NULL and node->_parent != NULL) {
			path.push_back(node->_label);
			id += node->_label * x, x *= 33;
			node = node->_parent;
		}

		if (reverse) std::reverse(path.begin(), path.end());
		return id;

	}

protected:

	T _label;
	uint _frequency;

	Node<T> *_child;
	Node<T> *_parent;
	Node<T> *_sibling;

	// Returns the node either found or inserted as child
	inline Node<T> * _getchild(const T& label, bool& allocated) {

		if (_child == NULL) {
			allocated = true;
			return _child = new Node<T>(label,
				reinterpret_cast<Node<T> *>(this));
		}

		allocated = false;
		Node<T> *child(_child);

		if (child->_label == label) return child;
		else while (child->_sibling != NULL) {
			if (child->_sibling->_label == label) {
				return child->_sibling;
			}
			child = child->_sibling;
		}

		allocated = true;
		return child->_sibling = new Node<T>(label,
				reinterpret_cast<Node<T> *>(this));

	}

};


template <template <typename> class Node, typename T>
class __Node__<Node, T>::iterator {

public:

	// Constructor
	iterator(Node<T> * const& root) :
	_changed(false), _node((root == NULL) ? NULL : root->_child),
	_level(0), _num_nodes(0) {

		if (_node == NULL) return;

		Node<T> *node(_node);
		while (node->_parent != NULL) ++_level, node = node->_parent;

	}

	// Copy constructor
	iterator(const iterator& it) :
	_changed(it._changed), _node(it._node),
	_level(it._level), _num_nodes(it._num_nodes) {
	}

	// Destructor
	~iterator() {
	}

	// Level
	inline const uint& level() const {
		return _level;
	}

	// Changed path
	inline const bool& changed() const {
		return _changed;
	}

	// Dereference operator
	inline Node<T> * operator*() const {
		return _node;
	}

	// Prefix increment -- advances the iterator in DFS
	inline void operator++() {

		_changed = false;
		if (_node == NULL) return;

		++_num_nodes;
		if (_node->_child != NULL) ++_level, _node = _node->_child;
		else if (_node->_sibling != NULL) {

			_changed = true;
			_node = _node->_sibling;

		} else {

			_changed = true;
			while (_node->_parent != NULL) {
				--_level, _node = _node->_parent;
				if (_node->_sibling != NULL) {
				   	_node = _node->_sibling;
					return;
				}
			}

			_node = NULL;

		}

	}

	// Postfix increment
	inline iterator operator++(int) {
		iterator it(*this);
	   	operator++();
		return it;
	}

	// Equality operator
	inline const bool operator==(const iterator& it) {
		return (_changed == it._changed
				and _node == it._node
				and _level == it._level
				and _num_nodes == it._num_nodes);
	}

	// Inequality operator
	inline const bool operator!=(const iterator& it) {
		return (_changed != it._changed
				or _node != it._node
				or _level != it._level
				or _num_nodes != it._num_nodes);
	}

private:

	bool _changed;
	Node<T> *_node;

	uint _level;
	uint _num_nodes;

};
