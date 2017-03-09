


/****************************************************

 Data structure c++11

 Mesh datastructure header file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

#ifndef CPP11_MESH_H
#define CPP11_MESH_H

#include <memory>
#include <vector>
#include <cassert>

#include "list.h"
#include "btree.h"
#include "utils.h"

/* The node */
template <class T, class KeyT>
class MNode : public std::enable_shared_from_this<LNode<T>>
{
public:
	/* Adjacent nodes */
	std::shared_ptr<MNode<T, KeyT>> rh;
	std::shared_ptr<MNode<T, KeyT>> rd;
	std::shared_ptr<MNode<T, KeyT>> dn;
	std::shared_ptr<MNode<T, KeyT>> lh;
	std::shared_ptr<MNode<T, KeyT>> lu;
	std::shared_ptr<MNode<T, KeyT>> up;

	/* Pointer to the data */
	std::shared_ptr<T> data;

	/* Key */
	KeyT key;

	/* Access... */
	void Set(T& d);
	T Get();

	/* Manipulation */
	int SetRH(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetRD(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetDN(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetLH(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetLU(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetUP(std::shared_ptr<MNode<T, KeyT>> mn);

	/* Constructors and Destructors */
	MNode();
	MNode(MNode<T, KeyT>& mn);
	virtual ~MNode() {;}
};

/* Key type for the node */
template <typename T>
struct NodeKey {
	T x;
	T y;

	/* Operator overloadings */
	bool operator== (const NodeKey<T>& o) const
	{
		if (x == o.x && y == o.y) return true;
		else return false;
	}

	bool operator!= (const NodeKey<T>& o) const
	{
		return !(this == o);
	}

	bool operator< (const NodeKey<T>& o) const
	{
		if ( x < o.x ) return true;
		else if ( x == o.x ) {
			if ( y < o.y ) return true;
			else return false;
		}
		else return false;
	}

	bool operator<= (const NodeKey<T>& o) const
	{
		return (this < o || this == o);
	}

	bool operator> (const NodeKey<T>& o) const
	{
		if ( x > o.x ) return true;
		else if ( x == o.x ) {
			if ( y > o.y ) return true;
			else return false;
		}
		else return false;
	}

	bool operator>= (const NodeKey<T>& o) const
	{
		return (this > o || this == 0);
	}
};



#include "mesh.tcc"

#endif /* Include guard */
