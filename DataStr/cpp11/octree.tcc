/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Octal Tree

  Implementation file

**********************************************/

/**********************************************
  Methods for OCTree
***********************************************/
/* Search... Let's not delve into too deep */
template <class T>
typename OCTree<T>::OCTreeNode* OCTree<T>::search(const uint64_t& index)
{
	/* Some edgy cases */
	if (!root_node) return nullptr;

	auto tmp_node = root_node.get();

	std::list<OCTreeNode*> other_nodes; /* FILO storage */

	while (tmp_node) {
		/* We've found it! */
		if (tmp_node->Index() == index) return tmp_node;

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
void OCTree<T>::Insert(const T& data, const uint64_t& index)
{
	/* Temporary storage for adjacent nodes */
	std::list<OCTreeNode*> current_nodes;

	/* New node */
	auto new_node = std::make_unique<OCTreeNode>(data, index);
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

		OCTreeNode *ll, *l, *r, *rr;
		auto tmp_it = tmp_node->Left.begin();

		if (index < tmp_node->index) {
			if (tmp_node->LFull()) tmp_it = tmp_node->Left.begin();
			else {
				new_node->parent = tmp_node;
				if (tmp_node->Left.empty()) depth++;
				tmp_node->Left.insert(std::move(new_node));
				n_nodes++;
				return;
			}
		}
		else {
			if (tmp_node->RFull()) tmp_it = tmp_node->Right.begin();
			else {
				new_node->parent = tmp_node;
				if (tmp_node->Right.empty()) depth++;
				tmp_node->Right.insert(std::move(new_node));
				n_nodes++;
				return;
			}
		}

		ll = (*tmp_it).get(); std::advance(tmp_it, 1);
		l = (*tmp_it).get(); std::advance(tmp_it, 1);
		r = (*tmp_it).get(); std::advance(tmp_it, 1);
		rr = (*tmp_it).get();

		if (index == ll->index) { ll->data = data; return; }
		if (index == l->index) { l->data = data; return; }
		if (index == r->index) { r->data = data; return; }
		if (index == rr->index) { rr->data = data; return; }

		if (index < ll->index) {
			tmp_node = ll;
			current_nodes.push_front(l);
			current_nodes.push_front(r);
			current_nodes.push_front(rr);
		}
		else if (ll->index < index && index < l->index) {
			if (l->LFull()) {
				tmp_node = ll;
				current_nodes.push_front(l);
				current_nodes.push_front(r);
				current_nodes.push_front(rr);
			}
			else {
				tmp_node = l;
				current_nodes.push_front(r);
				current_nodes.push_front(rr);
			}
			continue;
		}
		else if (l->index < index && index < r->index) {
			if (r->LFull()) {
				tmp_node = l;
				current_nodes.push_front(r);
				current_nodes.push_front(rr);
			}
			else {
				tmp_node = r;
				current_nodes.push_front(rr);
			}
			continue;
		}
		else if (r->index < index && index < rr->index) {
			if (rr->LFull()) {
				tmp_node = r;
				current_nodes.push_front(rr);
			}
			else tmp_node = rr;
			continue;
		}
		else {
			tmp_node = rr;
			continue;
		}

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
T& OCTree<T>::Get(const uint64_t& index)
{
	auto tmp_node = search(index);
	if (tmp_node) return tmp_node->data;
	else throw std::invalid_argument("OCTree: index can't be found!!");
}

/* Print */
template <class T>
void OCTree<T>::Print()
{
	uint64_t n = 1;
	for (auto it=indices.begin(); it!=indices.end(); ++it, ++n) {
		std::cout << "[" << n << "] " \
			<< "Key " << *it << ": " << Get(*it) << std::endl;
	}
}

/**********************************************
  Constructors and Destructors for OCTree
***********************************************/
template <class T>
OCTree<T>::OCTree() : depth(0), n_nodes(0), root_node(nullptr) {;}

template <class T>
OCTree<T>::~OCTree() {;}
