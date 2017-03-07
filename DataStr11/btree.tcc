/****************************************************

 Data structure c++11

 Binary tree

 Implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

/********** Node part ****************************/

/****************************************************
 BTNode::Access functions
*****************************************************/
template <class T, class KeyT>
T BTNode<T, KeyT>::Get() const
{
	return *data;
}
template <class T, class KeyT>
void BTNode<T, KeyT>::Set(T ndata)
{
	data = std::make_shared<T>(ndata);
}
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTNode<T, KeyT>::GetLeft() const
{
	return left;
}
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTNode<T, KeyT>::GetRight() const
{
	return right;
}
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTNode<T, KeyT>::GetParent() const
{
	return parent;
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetLeft(std::shared_ptr<BTNode<T, KeyT>> btnode)
{
	if (!btnode) {
		left = nullptr;
		return;
	}

	left = std::move(btnode);
	std::shared_ptr<BTNode<T, KeyT>> tmp = this;
	btnode->SetParent(tmp);
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetRight(std::shared_ptr<BTNode<T, KeyT>> btnode)
{
	if (!btnode) {
		right = nullptr;
		return;
	}

	right = std::move(btnode);
	std::shared_ptr<BTNode<T, KeyT>> tmp = this;
	btnode->SetParent(tmp);
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetParent(std::shared_ptr<BTNode<T, KeyT>> btnode)
{
	parent = std::move(btnode);
}
template <class T, class KeyT>
KeyT BTNode<T, KeyT>::GetKey() const
{
	return key;
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetKey(KeyT& newkey)
{
	key = newkey;
}

/****************************************************
 BTNode::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(T& ndata, KeyT& nkey)
{
	data = std::make_shared<T>(ndata);
	key = nkey;
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(BTNode<T, KeyT>& btnode)
{
	data = btnode->Get();
	left = btnode->GetLeft();
	right = btnode->GetRight();
	key = btnode->GetKey();
}

/********** Main part ****************************/
/****************************************************
 BTree::Private methods
*****************************************************/

/* Am I lefty? */
template <class T, class KeyT>
bool BTree<T, KeyT>::IsLefty(std::shared_ptr<BTNode<T, KeyT>> n)
{
	assert(n->GetParent() != nullptr);

	if (n->GetParent()->GetLeft() == n) return true;
	else return false;
}

/* Recursive style find */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::Find(
	KeyT& k, std::shared_ptr<BTNode<T, KeyT>> r)
{
	if (!r) return nullptr;

	std::shared_ptr<BTNode<T, KeyT>> tmp = std::move(r);

	if (tmp->GetKey() == k) return tmp;
	else {
		if (tmp->GetLeft()) Find(k, tmp->GetLeft());
		else if (tmp->GetRight()) Find(k, tmp->GetRight());
	}

	return nullptr;
}

/* Find minimum from given node */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::FindMin(
	std::shared_ptr<BTNode<T, KeyT>> r)
{
	if (r == nullptr) return nullptr;

	std::shared_ptr<BTNode<T, KeyT>> tmp = r->GetLeft();
	if (tmp) FindMin(tmp->GetLeft());
	else return tmp;
}

/* Traverse */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::Traverse(
	std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r)
{
	assert(bt != nullptr);

	if (r == bt) return std::move(r);

	if (r->GetLeft()) Traverse(bt, r->GetLeft());
	else if (r->GetRight()) Traverse(bt, r->GetRight());

	return nullptr;
}



/****************************************************
 BTree::Access functions
*****************************************************/
/* Get a value with a key */
template <class T, class KeyT>
T BTree<T, KeyT>::Get(KeyT& k) const
{
	return *this->Find(k, root_node);
}

/* Set a value with a key */
template <class T, class KeyT>
void BTree<T, KeyT>::Set(T& ndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = this->Find(k, root_node);
	tmp->Set(ndata);
}

/* Remove a node with a key */
template <class T, class KeyT>
bool BTree<T, KeyT>::RemNode(
	std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = Find(bt, r);

	if (!tmp) return false;

	/* Ok, we've found the target node */
	std::shared_ptr<BTNode<T, KeyT>> tmp_l = tmp->GetLeft();
	std::shared_ptr<BTNode<T, KeyT>> tmp_r = tmp->GetRight();
	std::shared_ptr<BTNode<T, KeyT>> tmp_p = tmp->GetParent();
	std::shared_ptr<BTNode<T, KeyT>> tmp_min;
	std::shared_ptr<BTNode<T, KeyT>> tmp_min_p;
	bool me_left;

	/* If I don't have any parent ... I am the root @.@!! */
	if (tmp_p == nullptr) {
		if (!tmp_l && !tmp_r) delete tmp;
		else if (tmp_l && !tmp_r) {
			root_node = std::move(tmp_l);
		}
		else if (!tmp_l && tmp_r) {
			root_node = std::move(tmp_r);
		}
		else if (tmp_l && tmp_r) {
			/* Find minimum fron right side */
			tmp_min = FindMin(tmp_r);
			// tmp_min must be lefty
			tmp_min_p = tmp_min->GetParent();
			tmp_min_p->SetLeft(nullptr);
			/* Then make it root */
			root_node = std::move(tmp_min);
			tmp_min->SetParent(nullptr);
			tmp_min->SetLeft(tmp_l);
			tmp_min->SetRight(tmp_r);
		}
		root_node->SetParent(nullptr);
		delete tmp;
		return true;
	}

	/* Let's find out my identity */
	me_left = IsLefty(tmp);

	/* Now, how many chilren do I have? */
	if (!tmp_l && !tmp_r) {
		if (me_left) tmp_p->SetLeft(nullptr);
		else tmp_p->SetRight(nullptr);
	}
	else if (tmp_l && !tmp_r) {
		if (me_left) tmp_p->SetLeft(tmp_l);
		else tmp_p->SetRight(tmp_l);
	}
	else if (!tmp_l && tmp_r) {
		if (me_left) tmp_p->SetLeft(tmp_r);
		else tmp_p->SetRight(tmp_r);
	}
	else if (tmp_l && tmp_r) {
		tmp_min = FindMin(tmp_r);
		tmp_min_p = tmp_min->GetParent();
		tmp_min_p->SetLeft(nullptr);

		if (me_left) tmp_p->SetLeft(tmp_min);
		else tmp_p->SetRight(tmp_min);

		tmp_min->SetLeft(tmp_l);
		tmp_min->SetRight(tmp_r);
	}

	delete tmp;
	return true;
}

/* Insert node into given root node */
template <class T, class KeyT>
bool BTree<T, KeyT>::InsNode(
	std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r)
{
	if (!r) {
		r = std::move(bt);
		return true;
	}

	/* bt is smaller key: push to left */
	if ( bt->GetKey() <= r->GetKey() ) {
		if (!r->GetLeft()) r->SetLeft(bt);
		else InsNode(bt, r->GetLeft());
	}
	/* Other than that, push to right */
	else {
		if (!r->GetRight()) r->SetRight(bt);
		else InsNode(bt, r->GetRight());
	}
	return true;
}


/****************************************************
 BTree::Manipulation
*****************************************************/
template <class T, class KeyT>
void BTree<T, KeyT>::Insert(T& ndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = \
		std::make_shared<BTNode<T, KeyT>>(ndata, k);

	if (!root_node) {
		root_node = std::move(tmp);
		return;
	}

	InsNode(tmp, root_node);
}

template <class T, class KeyT>
void BTree<T, KeyT>::Remove(KeyT& k)
{
	if (!root_node) return;

	std::shared_ptr<BTNode<T, KeyT>> tmp = \
		this->Find(k, root_node);

	RemNode(tmp, root_node);
}


/****************************************************
 BTree::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
BTree<T, KeyT>::BTree(BTree<T, KeyT>& bt)
{
	root_node = bt.root_node;
	nodes = bt.nodes;
}
