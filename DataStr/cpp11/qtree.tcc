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

	auto tmp_node = root_node.get();

	std::list<QTreeNode*> other_nodes; /* FILO storage */

	while (tmp_node) {

		if (tmp_node->index == index) return tmp_node;

		/* Find Left */
		if (!tmp_node->Left.empty()) {
			for (auto l=tmp_node->Left.begin(); l!=tmp_node->Left.end(); ++l) {
				if ((*l)->index == index) return (*l).get();
				other_nodes.push_front((*l).get());
			}
		}

		/* Find Right */
		if (!tmp_node->Right.empty()) {
			for (auto r=tmp_node->Right.begin(); r!=tmp_node->Right.end(); ++r) {
				if ((*r)->index == index) return (*r).get();
				other_nodes.push_front((*r).get());
			}
		}

		/* If index can't be found in this node, let's try other node. */
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
	/* Temporary storage for adjacent nodes */
	std::list<QTreeNode*> current_nodes;

	/* New node */
	auto new_node = std::make_unique<QTreeNode>(data, index);
	indices.insert(index);

	/* Edge case */
	if (!root_node) {
		root_node = std::move(new_node);
		n_nodes++;
		return;
	}

	/* Now travel down */
	auto tmp_node = root_node.get();
	if (!depth) depth++;

	while (tmp_node) {

		/* Found the node! */
		if (tmp_node->index == index) {
			tmp_node->data = data;
			return;
		}

		/* Push into left */
		if (index < tmp_node->index) {

			/* Full node. update tmp_node and insert other nodes to temporary storage: current_nodes */
			if (tmp_node->LFull()) {
				auto i_left = tmp_node->Left.begin();
				auto i_right = i_left;
				std::advance(i_right, 1);

				auto s_left = (*i_left).get();
				auto s_right = (*i_right).get();

				if (index < s_left->index) {
					current_nodes.push_front(s_right);
					tmp_node = s_left;
				}
				else if (s_left->index < index && index < s_right->index) {
					if (s_left->RFull()) {
						tmp_node = s_right;
					}
					else {
						current_nodes.push_front(s_right);
						tmp_node = s_left;
					}
				}
				else if (s_right->index < index) {
					tmp_node = s_right;
				}
				else if (index == s_left->index) {
					s_left->data = data;
					break;
				}
				else if (index == s_right->index) {
					s_right->data = data;
					break;
				}
				continue;

			} /* if (tmp_node->LFull()) */
			/* If left side is not full, just put this thing in... <set>
			will re-arrange it itself */
			else {
				new_node->parent = tmp_node;
				tmp_node->Left.insert(std::move(new_node));
				break;
			}

		} /* if (index < tmp_node->index) { */

		/* Push into right */
		else {
			/* Full node. update tmp_node and insert other nodes to temporary storage: current_nodes */
			if (tmp_node->RFull()) {
				auto i_left = tmp_node->Right.begin();
				auto i_right = i_left;
				std::advance(i_right, 1);

				auto s_left = (*i_left).get();
				auto s_right = (*i_right).get();

				if (index < s_left->index) {
					current_nodes.push_front(s_right);
					tmp_node = s_left;
				}
				else if (s_left->index < index && index < s_right->index) {
					if (s_left->RFull()) {
						tmp_node = s_right;
					}
					else {
						current_nodes.push_front(s_right);
						tmp_node = s_left;
					}
				}
				else if (s_right->index < index) {
					tmp_node = s_right;
				}
				else if (index == s_left->index) {
					s_left->data = data;
					break;
				}
				else if (index == s_right->index) {
					s_right->data = data;
					break;
				}
				continue;

			} /* if (tmp_node->RFull()) */
			/* If left side is not full, just put this thing in... <set>
			will re-arrange it itself */
			else {
				new_node->parent = tmp_node;
				tmp_node->Right.insert(std::move(new_node));
				break;
			}
		} /* if (index < tmp_node->index) else { */

		/* If we couldn't find any vacancy here... */
		if (!current_nodes.empty()) {
			tmp_node = current_nodes.back();
			current_nodes.pop_back();
		}
		else tmp_node = nullptr;
	} /* while (tmp_node) { */

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

/* Print all the stuff we have */
template <class T>
void QTree<T>::Print()
{
	uint64_t n = 1;
	for (auto it=indices.begin(); it!=indices.end(); ++it, ++n) {
		std::cout << "[" << n << "] " \
			<< "Key " << *it << ": " << Get(*it) << std::endl;
	}
}

/**********************************************
  Constructors and Destructors for QTree
***********************************************/
template <class T>
QTree<T>::QTree() : depth(0), n_nodes(0), root_node(nullptr), indices() {;}

template <class T>
QTree<T>::~QTree() {;}
