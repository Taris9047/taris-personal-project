/****************************************************

 Data structure c++11

 Mesh structure implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

/****************************************************
 MNode::Access node methods
*****************************************************/
template <class T, class KeyT>
void MNode<T, KeyT>::Set(T& d)
{
	std::shared_ptr<T> sd = \
		std::make_shared<T>(d);
	assert(sd);
	this->data = sd;
}

template <class T, class KeyT>
T MNode<T, KeyT>::Get()
{
	assert(this->data);
	return *this->data;
}

template <class T, class KeyT>
std::shared_ptr<T> MNode<T, KeyT>::pGet()
{
	return this->data;
}

/****************************************************
 MNode::Manipulation
*****************************************************/
template <class T, class KeyT>
int MNode<T, KeyT>::SetRH(std::shared_ptr<MNode<T, KeyT>> mn)
{
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetRD(std::shared_ptr<MNode<T, KeyT>> mn)
{
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetDN(std::shared_ptr<MNode<T, KeyT>> mn)
{
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetLH(std::shared_ptr<MNode<T, KeyT>> mn)
{
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetLU(std::shared_ptr<MNode<T, KeyT>> mn)
{
}

template <class T, class KeyT>
int MNode<T, KeyT>::SetUP(std::shared_ptr<MNode<T, KeyT>> mn)
{
}


template <class T, class KeyT>
void MNode<T, KeyT>::SetKey(KeyT& k)
{
	this->key = k;
}

template <class T, class KeyT>
KeyT MNode<T, KeyT>::GetKey()
{
	return this->key;
}

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
