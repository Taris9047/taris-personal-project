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
void BTNode<T, KeyT>::SetLeft(std::shared_ptr<BTNode<T, KeyT>>& btnode)
{
	left = std::move(btnode);
}
template <class T, class KeyT>
void BTNode<T, KeyT>::SetRight(std::shared_ptr<BTNode<T, KeyT>>& btnode)
{
	right = std::move(btnode);
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
BTNode<T, KeyT>::BTNode(T ndata, KeyT& nkey)
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
 BTree::Access functions
*****************************************************/


/****************************************************
 BTree::Manipulation
*****************************************************/


/****************************************************
 BTree::Constructors and Destructors
*****************************************************/
