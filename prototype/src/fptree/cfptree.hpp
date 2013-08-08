# include <vector>
# include <sstream>
# include <utility>
# include <iostream>
# include <algorithm>
# include <unordered_map>
# include "cnode.hpp"
# include "__fptree__.hpp"


template <typename T>
class CFPTree : public __FPTree__<Node, T> {

public:

	// Constructor
	CFPTree(const T& label = T()) :
	__FPTree__<Node, T>(label) {
	}

	// Destructor
	~CFPTree() {

		for (auto it(_link.begin()); it != _link.end(); ++it) {
			it->second.clear();
		}

		_link.clear();
		_node.clear();
		_prefix.clear();

	}

	// Initializes root nodes
	inline void init(const uint& length) {

		for (uint i(0); i < length; ++i) {
			_node.push_back(std::make_pair(0, &_root));
		}

	}

	// Merges local FPTrees
	inline void merge(const uint& source, const T& label,
			const uint& frequency, const uint& level = 0) {

		auto& it(_node[source]);
		while (level and it.first >= level) {
			--it.first, it.second = it.second->parent();
		}

		bool allocated(false);
		it.second = it.second->insert(label, allocated, frequency);
		++it.first;

		if (level == 1) _root.add_frequency(frequency);
		if (allocated) {
			_link[label].push_back(it.second), ++_num_nodes;
		}

	}

	// Projects the local FPTree
	template <class DP, class DS>
	inline void project(DP&& Pack, DS&& Signal, const uint& level) {

		for (auto it(_link.begin()); it != _link.end(); ++it) {
			_key.push_back(it->first);
		}
		std::sort(_key.rbegin(), _key.rend());

		T label;
		uint id(0), frequency(0);

		CFPTree<T> *cfptree(_project());
		while (cfptree != NULL) {

			Node<T> *node(&cfptree->_root); id = node->label() * 33;
			Signal(id, node->label(), node->frequency());

			typename Node<T>::iterator it(&cfptree->_root);
			while (node = *it, node != NULL) {

				label = node->label(), frequency = node->frequency();
				if (!it.changed()) Pack(id, label, frequency, false);
				else Pack(id, label, frequency, true, it.level());
				++it;

			}

			delete cfptree, cfptree = _project();

		}

	}

	// Sets the initial 1-item set frequency
	inline void init(const T& label, const uint& frequency) {

		_root.add_frequency(frequency);
		if (_prefix.empty()) {
			_root.set_label(label);
			_prefix.push_back(label);
		}

	}

	// FPGrowth algorithm
	inline void fpgrowth(const float& support, std::ostream& out) {

		std::vector<CFPTree<T> *> tree(1, this);
		do {

			CFPTree *cur(tree.back());
			if (cur->_single_path()) cur->_itemsets(out, support);
			else {

				CFPTree *cond(cur->_project(support));
				if (cond == NULL) cur->_itemsets(out, support, true);
				else {
					tree.push_back(cond);
					continue;
				}

			}

			if (cur != this) delete cur;
			tree.pop_back();

		} while (!tree.empty());
		out.flush();

	}

	// Frees the entire memory pool
	using __FPTree__<Node, T>::static_free;

	// Frees the entire memory pool
	using __FPTree__<Node, T>::free;

	// Number of nodes in the tree
	using __FPTree__<Node, T>::nodecount;

private:

	using __FPTree__<Node, T>::_root;
	using __FPTree__<Node, T>::_num_nodes;

	std::vector<T> _key, _prefix;
	std::vector<std::pair<uint, Node<T> *> > _node;
	std::unordered_map<T, std::vector<Node<T> *> > _link;

	// Constructor
	CFPTree(const T& label,	const std::vector<T>& prefix) :
	__FPTree__<Node, T>(label), _prefix(prefix) {
		_prefix.push_back(label);
	}

	// Projects the current CFPTree using ordered keys
	inline CFPTree<T> * _project() {

		if (_key.empty()) return NULL;

		const auto it(_link.find(_key.back()));
		CFPTree<T> *cfptree(_project(0, *it, false));
		it->second.clear(), _link.erase(it), _key.pop_back();
		return cfptree;

	}

	// Projects the current CFPTree
	inline CFPTree<T> * _project(const float& support) {

		auto it(_link.begin());
		while (it != _link.end()) {

			uint freq(0);
			for (uint i(0); i < it->second.size(); ++i) {
				freq += it->second[i]->frequency();
			}

			if (freq and freq >= support) {
				CFPTree<T> *cfptree(_project(freq, *it, true));
				it->second.clear(), _link.erase(it);
				return cfptree;
			}
			it->second.clear(), it = _link.erase(it);

		}

		return NULL;

	}

	// Projects the current CFPTree on the given label
	inline CFPTree<T> * _project(const uint& frequency,
			const std::pair<T, std::vector<Node<T> *> >& it,
			const bool& linkage) {

		CFPTree<T> *cfptree(new CFPTree<T>(it.first, _prefix));
		if (linkage) cfptree->_root.add_frequency(frequency);

		const auto& link(it.second);
		for (uint i(0); i < link.size(); ++i) {

			std::vector<T> path;
			link[i]->recover(path, false);
			uint local_freq(link[i]->frequency());

			Node<T> *node(&cfptree->_root);
			if (!linkage) node->add_frequency(local_freq);

			for (uint j(path.size()); j > 1; --j) {

				bool allocated(false);
				const auto& id(path[j - 1]);
				node = node->insert(id, allocated, local_freq);

				if (!linkage) continue;
				if (allocated) cfptree->_link[id].push_back(node);

			}

		}

		return cfptree;

	}

	// States whether the current CFPTree has a single path
	inline const bool _single_path() {
		return _root.single_path();
	}

	// Generate item sets
	inline void _itemsets(std::ostream& outstream,
			const float& support = 0,
			const bool& prefix_only = false) {

		std::vector<T> item;
		std::vector<uint> frequency;
		if (prefix_only) {
			_itemsets(item, frequency, outstream);
			return;
		}

		typename Node<T>::iterator it(&_root);
		Node<T> *node(*it);

		while (node != NULL and node->frequency() >= support) {
			item.push_back(node->label());
			frequency.push_back(node->frequency());
			++it, node = *it;
		}

		_itemsets(item, frequency, outstream);
		item.clear(), frequency.clear();

	}

	// Writes down the item sets
	inline void _itemsets(const std::vector<T>& item,
			const std::vector<uint>& frequency,
			std::ostream& outstream, const char& delim = '\t',
			const char& inner_sep = ' ') {

		std::stringstream ss; ss << _prefix[0];
		for (uint i(1); i < _prefix.size(); ++i) {
			ss << inner_sep << _prefix[i];
		}

		outstream << _root.frequency() << delim << ss.str() << '\n';
		if (item.empty()) return;

		uint i(0), size(item.size());
		std::vector<uint> pos(size + 1, 0);
		while (true) {

			if (pos[i] < size) pos[i + 1] = pos[i] + 1, ++i;
			else ++pos[i - 1], --i;
			if (i == 0) break;

			outstream << frequency[pos[i] - 1] << delim << ss.str();
			for (uint j(1); j <= i; ++j) {
				outstream << inner_sep << item[pos[j] - 1];
			}
			outstream << '\n';

		}

	}

};
