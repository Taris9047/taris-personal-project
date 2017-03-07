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
Key<KeyT> MNode<T, KeyT>::GetCoords() const
{
	return coords;
}
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::GetRH() const
{
	return adjnodes[0];
}
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::GetRD() const
{
	return adjnodes[1];
}
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::GetDN() const
{
	return adjnodes[2];
}
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::GetLH() const
{
	return adjnodes[3];
}
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::GetLU() const
{
	return adjnodes[4];
}
template <class T, class KeyT>
std::shared_ptr<MNode<T, KeyT>> MNode<T, KeyT>::GetUP() const
{
	return adjnodes[5];
}

template <class T, class KeyT>
void MNode<T, KeyT>::SetCoords(Key<KeyT>& k)
{
	coords = k;
}
template <class T, class KeyT>
void MNode<T, KeyT>::SetRH(std::shared_ptr<MNode<T, KeyT>> pmn)
{
	adjnodes[0] = std::move(pmn);
}
template <class T, class KeyT>
void MNode<T, KeyT>::SetRD(std::shared_ptr<MNode<T, KeyT>> pmn)
{
	adjnodes[1] = std::move(pmn);
}
template <class T, class KeyT>
void MNode<T, KeyT>::SetDN(std::shared_ptr<MNode<T, KeyT>> pmn)
{
	adjnodes[2] = std::move(pmn);
}
template <class T, class KeyT>
void MNode<T, KeyT>::SetLH(std::shared_ptr<MNode<T, KeyT>> pmn)
{
	adjnodes[3] = std::move(pmn);
}
template <class T, class KeyT>
void MNode<T, KeyT>::SetLU(std::shared_ptr<MNode<T, KeyT>> pmn)
{
	adjnodes[4] = std::move(pmn);
}
template <class T, class KeyT>
void MNode<T, KeyT>::SetUP(std::shared_ptr<MNode<T, KeyT>> pmn)
{
	adjnodes[5] = std::move(pmn);
}

/****************************************************
 MNode::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
MNode<T, KeyT>::MNode()
{
	data = nullptr;
	coords = Key<T>{0, 0};
	adjnodes.reserve(6);

	int i;
	for (i=0; i<6; ++i) adjnodes[i] = nullptr;
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(T& d, Key<KeyT>& k) : MNode()
{
	data = std::shared_ptr<T>(d);
	coords = k;
}

template <class T, class KeyT>
MNode<T, KeyT>::MNode(MNode<T, KeyT>& mn)
{
	data = mn.data;
	coords = mn.coords;
	adjnodes = mn.adjnodes;
}

template <class T, class KeyT>
MNode<T, KeyT>::~MNode()
{

}
