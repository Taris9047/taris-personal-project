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
	btnode->parent = this->shared_from_this();
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetRight(std::shared_ptr<BTNode<T, KeyT>> btnode)
{
	if (!btnode) {
		right = nullptr;
		return;
	}

	right = std::move(btnode);
	btnode->parent = this->shared_from_this();
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
BTNode<T, KeyT>::BTNode()
{
	data = nullptr;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(T& ndata, KeyT& nkey) : BTNode()
{
	data = std::make_shared<T>(ndata);
	key = nkey;
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(BTNode<T, KeyT>& btnode)
{
	data = btnode->Get();
	left = btnode->left;
	right = btnode->right;
	parent = btnode->parent;
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

	if (n->parent->left == n) return true;
	else return false;
}

/* Recursive style find */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::Find(
	KeyT& k, std::shared_ptr<BTNode<T, KeyT>> r)
{
	if (r == nullptr) return nullptr;

	std::shared_ptr<BTNode<T, KeyT>> tmp = std::move(r);

	if (tmp->GetKey() == k) return tmp;

	if (tmp->GetKey() > k) tmp = Find(k, tmp->left);
	else tmp = Find(k, tmp->right);
	return tmp;
}

/* Find minimum from given node */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::FindMin(
	std::shared_ptr<BTNode<T, KeyT>> r)
{
	if (r == nullptr) return nullptr;

	std::shared_ptr<BTNode<T, KeyT>> tmp = r;

	while (tmp->left) tmp = tmp->left;
	return tmp;
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

/* Same as Get but returns pointer */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::pGet(KeyT& k)
{
	return this->Find(k, root_node);
}

/* Set a value with a key */
template <class T, class KeyT>
void BTree<T, KeyT>::Set(T& ndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = Find(k, root_node);
	tmp->Set(ndata);
}

/* Remove a node with a key */
template <class T, class KeyT>
bool BTree<T, KeyT>::RemNode(
	std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r)
{
	KeyT tmp_key = bt->GetKey();
	std::shared_ptr<BTNode<T, KeyT>> tmp = Find(tmp_key, r);

	if (!tmp) return false;

	/* Ok, we've found the target node */
	std::shared_ptr<BTNode<T, KeyT>> tmp_l = tmp->left;
	std::shared_ptr<BTNode<T, KeyT>> tmp_r = tmp->right;
	std::shared_ptr<BTNode<T, KeyT>> tmp_p = tmp->parent;
	std::shared_ptr<BTNode<T, KeyT>> tmp_min;
	std::shared_ptr<BTNode<T, KeyT>> tmp_min_p;
	bool me_left;

	/* If I don't have any parent ... I am the root @.@!! */
	if (tmp_p == nullptr) {
		if (!tmp_l && !tmp_r) tmp.reset();
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
			tmp_min_p = tmp_min->parent;
			tmp_min_p->left = nullptr;
			/* Then make it root */
			root_node = std::move(tmp_min);
			root_node->parent = nullptr;
			root_node->left = tmp_l;
			root_node->right = tmp_r;
		}
		root_node->parent = nullptr;
		tmp.reset();
		return true;
	}

	/* Let's find out my identity */
	me_left = IsLefty(tmp);

	/* Now, how many chilren do I have? */
	if (!tmp_l && !tmp_r) {
		if (me_left) tmp_p->left = nullptr;
		else tmp_p->right = nullptr;
	}
	else if (tmp_l && !tmp_r) {
		if (me_left) tmp_p->left = tmp_l;
		else tmp_p->right = tmp_l;
	}
	else if (!tmp_l && tmp_r) {
		if (me_left) tmp_p->left = tmp_r;
		else tmp_p->right = tmp_r;
	}
	else if (tmp_l && tmp_r) {
		tmp_min = FindMin(tmp_r);
		tmp_min_p = tmp_min->parent;
		tmp_min_p->left = nullptr;

		if (me_left) tmp_p->left = tmp_min;
		else tmp_p->right = tmp_min;

		tmp_min->left = tmp_l;
		tmp_min->right = tmp_r;
	}

	tmp.reset();
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
	if ( bt->GetKey() < r->GetKey() ) {
		if (!r->left) {
			r->left = bt;
			bt->parent = r;
		}
		else InsNode(bt, r->left);
	}
	/* Other than that, push to right */
	else {
		if (!r->right) {
			r->right = bt;
			bt->parent = r;
		}
		else InsNode(bt, r->right);
	}
	return true;
}


/****************************************************
 BTree::Manipulation
*****************************************************/
template <class T, class KeyT>
int BTree<T, KeyT>::Insert(T& ndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = \
		std::make_shared<BTNode<T, KeyT>>(ndata, k);

	if (!root_node) {
		root_node = std::move(tmp);
		return -1;
	}

	return InsNode(tmp, root_node);
}

template <class T, class KeyT>
int BTree<T, KeyT>::Remove(KeyT& k)
{
	if (!root_node) return -1;

	std::shared_ptr<BTNode<T, KeyT>> tmp = \
		this->Find(k, root_node);

	if (!tmp) return -1;

	return RemNode(tmp, root_node);
}


/****************************************************
 BTree::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
BTree<T, KeyT>::BTree(BTree<T, KeyT>& bt) : BTree()
{
	root_node = bt.root_node;
	nodes = bt.nodes;
}
