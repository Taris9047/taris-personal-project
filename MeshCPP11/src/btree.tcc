/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:16:42-06:00
*/



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
T& BTNode<T, KeyT>::Get()
{
	return *data;
}

template <class T, class KeyT>
int BTNode<T, KeyT>::Set(T ndata)
{
	data = std::make_shared<T>(ndata);
	return 0;
}
template <class T, class KeyT>
int BTNode<T, KeyT>::Set(std::shared_ptr<T> pndata)
{
	data = pndata;
	return 0;
}
template <class T, class KeyT>
KeyT& BTNode<T, KeyT>::GetKey()
{
	return key;
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetKey(KeyT& newkey)
{
	key = newkey;
}



/****************************************************
 BTNode::Operator Overloading
*****************************************************/
template <class T, class KeyT>
BTNode<T, KeyT>& BTNode<T, KeyT>::operator= (const BTNode<T, KeyT>& btnode)
{
	BTNode<T, KeyT> tmp(btnode);
	*this = std::move(tmp);
	return *this;
}

template <class T, class KeyT>
BTNode<T, KeyT>& BTNode<T, KeyT>::operator= (BTNode<T, KeyT>&& btnode) noexcept
{
	data = nullptr;
	left = nullptr;
	right = nullptr;
	//parent = nullptr;

	data = std::move(btnode.data);
	left = std::move(btnode.left);
	right = std::move(btnode.right);
	parent = std::move(btnode.parent);

	return *this;
}

/****************************************************
 BTNode::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
BTNode<T, KeyT>::BTNode() : \
	data(nullptr),
	left(nullptr),
	right(nullptr),
	parent(nullptr)
{
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(T& ndata, KeyT& nkey) : BTNode()
{
	data = std::make_shared<T>(ndata);
	key = nkey;
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(std::shared_ptr<T> pndata, KeyT& nkey)
{
	data = pndata;
	key = nkey;
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(const BTNode<T, KeyT>& btnode)
{
	T& tmp_data = *btnode->Get();
	BTNode<T, KeyT>& tmp_l = *btnode->left;
	BTNode<T, KeyT>& tmp_r = *btnode->right;
	BTNode<T, KeyT>& tmp_p = *btnode->parent;

	data = std::make_shared<T>(tmp_data);
	left = std::make_shared<BTNode<T, KeyT>>(tmp_l);
	right = std::make_shared<BTNode<T, KeyT>>(tmp_r);
	parent = std::make_shared<BTNode<T, KeyT>>(tmp_p);

	key = btnode->GetKey();
}

template <class T, class KeyT>
BTNode<T, KeyT>::BTNode(BTNode<T, KeyT>&& btnode) noexcept
{
	data = std::move(btnode->data);
	left = std::move(btnode->left);
	right = std::move(btnode->right);
	parent = std::move(btnode->parent);
	key = btnode->GetKey();
}

template <class T, class KeyT>
BTNode<T, KeyT>::~BTNode()
{
	data = nullptr;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}






























/********** Main part ****************************/
/****************************************************
 BTree::Private methods
*****************************************************/

/* Am I lefty? */
template <class T, class KeyT>
bool BTree<T, KeyT>::IsLefty(std::shared_ptr<BTNode<T, KeyT>> n)
{
	assert(n->parent);

	if (n->parent->left == n) return true;
	else return false;
}

/* Recursive style find */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::Find(
	KeyT& k, std::shared_ptr<BTNode<T, KeyT>> r)
{
	if (r == nullptr) return nullptr;

	KeyT tmp_k = r->GetKey();

	if (tmp_k == k) return r;
	else if (tmp_k > k) return Find(k, r->left);
	else if (tmp_k < k) return Find(k, r->right);

	return nullptr;
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
T& BTree<T, KeyT>::Get(KeyT& k)
{
	T& tmp = (this->Find(k, root_node))->Get();
	return tmp;
}

/* Same as Get but returns pointer */
template <class T, class KeyT>
std::shared_ptr<BTNode<T, KeyT>> BTree<T, KeyT>::pGet(KeyT& k)
{
	return this->Find(k, root_node);
}

/* Set a value with a key */
template <class T, class KeyT>
int BTree<T, KeyT>::Set(T& ndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = Find(k, root_node);
	return tmp->Set(ndata);
}

template <class T, class KeyT>
int BTree<T, KeyT>::Set(std::shared_ptr<T> pndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = Find(k, root_node);
	return tmp->Set(pndata);
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
	assert(bt);

	if (!r) {
		r = std::move(bt);
		return true;
	}

	KeyT bt_k = bt->GetKey();
	KeyT r_k = r->GetKey();

	/* if r and bt has the same key
	  Overwrite r's value. */
	if ( bt_k == r_k ) {
		r->data = bt->data;
	}
	/* bt is smaller key: push to left */
	else if ( bt_k < r_k ) {
		if (!r->left) {
			r->left = bt;
			bt->parent = r;
		}
		else InsNode(bt, r->left);
	}
	/* Other than that, push to right */
	else if ( bt_k > r_k ) {
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
		return 1;
	}

	return InsNode(tmp, root_node);
}

template <class T, class KeyT>
int BTree<T, KeyT>::Insert(std::shared_ptr<T> pndata, KeyT& k)
{
	std::shared_ptr<BTNode<T, KeyT>> tmp = \
		std::make_shared<BTNode<T, KeyT>>(pndata, k);

	if (!root_node) {
		root_node = std::move(tmp);
		return 1;
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
 BTree::Operator overloading
*****************************************************/
template <class T, class KeyT>
BTree<T, KeyT>& BTree<T, KeyT>::operator= (const BTree<T, KeyT>& bt)
{
	BTree<T, KeyT> tmp(bt);
	*this = std::move(tmp);
	return *this;
}

template <class T, class KeyT>
BTree<T, KeyT>& BTree<T, KeyT>::operator= (BTree<T, KeyT>&& bt) noexcept
{
	root_node = std::move(bt.root_node);
	bt.data = nullptr;
	nodes = bt.nodes;
	return *this;
}

/****************************************************
 BTree::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
BTree<T, KeyT>::BTree(const BTree<T, KeyT>& bt) : BTree()
{
	BTNode<T, KeyT>& tmp_n = *bt.root_node;
	root_node = std::make_shared<BTNode<T, KeyT>>(tmp_n);
	nodes = bt.nodes;
}

template <class T, class KeyT>
BTree<T, KeyT>::BTree(BTree<T, KeyT>&& bt) noexcept
{
	root_node = std::move(bt.root_node);
	nodes = bt.nodes;
}

template <class T, class KeyT>
BTree<T, KeyT>::~BTree()
{
	root_node = nullptr;
	nodes = 0;
}
