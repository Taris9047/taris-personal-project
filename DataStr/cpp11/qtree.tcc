/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Quad Tree

  Implementation file

**********************************************/

/**********************************************
  Methods for QTree
***********************************************/
/* Search... Let's not delve into too deep */
template <class T>
typename QTree<T>::QTreeNode* QTree<T>::search(const uint64_t& index)
{
	/* Some edgy cases */
	if (!root_node) return nullptr;
	if (!index) return root_node.get();

	auto tmp_node = root_node.get();

	std::list<QTreeNode*> other_nodes; /* FILO storage */

	while (tmp_node) {
		if (tmp_node->NE) {
			if (tmp_node->NE->index == index) return tmp_node->NE.get();
			other_nodes.push_front(tmp_node->NE.get());
		}
		if (tmp_node->NW) {
			if (tmp_node->NW->index == index) return tmp_node->NW.get();
			other_nodes.push_front(tmp_node->NW.get());
		}
		if (tmp_node->SE) {
			if (tmp_node->SE->index == index) return tmp_node->SE.get();
			other_nodes.push_front(tmp_node->SE.get());
		}
		if (tmp_node->SW) {
			if (tmp_node->SW->index == index) return tmp_node->SW.get();
			other_nodes.push_front(tmp_node->SW.get());
		}

		// If index can't be found in this node, let's try other node.
		if (!other_nodes.empty()) {
		    tmp_node = other_nodes.back();
		    other_nodes.pop_back();
		}
		else tmp_node = nullptr;
	}

	return nullptr;
}

/* Insert stuff */
template <class T>
void QTree<T>::Insert(const T& data, const uint64_t& index)
{
	auto found_node = search(index);

	std::list<QTreeNode*> current_nodes;

	/* We've found a node with the index!! */
	if (found_node) {
		found_node->data = data;
		return;
	}

	/* Couldn't find the index node */
	auto new_node = std::make_unique<QTreeNode>(data, index);

	/* Now travel down */
	auto tmp_node = root_node.get();
	while (tmp_node) {
		if (!tmp_node->NW) {
			tmp_node->NW = std::move(new_node);
			tmp_node->NW->parent = tmp_node;
			n_nodes++;
			return;
		}
		else current_nodes.push_front(tmp_node->NW.get());

		if (!tmp_node->NE) {
			tmp_node->NE = std::move(new_node);
			tmp_node->NE->parent = tmp_node;
			n_nodes++;
			return;
		}
		else current_nodes.push_front(tmp_node->NE.get());

		if (!tmp_node->SW) {
			tmp_node->SW = std::move(new_node);
			tmp_node->SW->parent = tmp_node;
			n_nodes++;
			return;
		}
		else current_nodes.push_front(tmp_node->SW.get());

		if (!tmp_node->SE) {
			tmp_node->SE = std::move(new_node);
			tmp_node->SE->parent = tmp_node;
			n_nodes++;
			return;
		}
		else current_nodes.push_front(tmp_node->SE.get());

		/* If we couldn't find any vacancy here... */
		if (!current_nodes.empty()) {
		    tmp_node = current_nodes.back();
		    current_nodes.pop_back();
		    depth++;
		}
		else tmp_node = nullptr;
	}

	return;
}

/* Get stuff */
template <class T>
T& QTree<T>::Get(const uint64_t& index)
{
	auto tmp_node = search(index);
	if (tmp_node) return tmp_node->data;
	else throw std::invalid_argument("QTree: index can't be found!!");
}

/**********************************************
  Constructors and Destructors for QTree
***********************************************/
template <class T>
QTree<T>::QTree() : depth(0), n_nodes(0), root_node(nullptr) {;}

template <class T>
QTree<T>::~QTree() {;}
