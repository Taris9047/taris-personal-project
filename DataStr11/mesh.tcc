/****************************************************

 Data structure c++11

 Mesh structure implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

/****************************************************
 MNode::Access node methods
*****************************************************/


/****************************************************
 MNode::Manipulation
*****************************************************/


/****************************************************
 MNode::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
MNode<T, KeyT>::MNode() :
	rh(nullptr),
	rd(nullptr),
	dn(nullptr),
	lh(nullptr),
	lu(nullptr),
	up(nullptr),
	data(nullptr)
{
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(MNode<T, KeyT>& mn) :
	rh(mn.rh),
	rd(mn.rd),
	dn(mn.dn),
	lh(mn.lh),
	lu(mn.lu),
	up(mn.up),
	data(mn.data),
	key(mn.key)
{
}
