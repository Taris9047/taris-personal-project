/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:17:06-06:00
*/



/****************************************************

 Data structure c++11

 Binary tree

 header file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/
#ifndef CPP11_BTREE_H
#define CPP11_BTREE_H

#include <memory>
#include <cassert>

#include "utils.hpp"

/* Binary tree node class */
template <class T, class KeyT>
class BTNode : public std::enable_shared_from_this<BTNode<T, KeyT>>
{
public:
    std::shared_ptr<T> data; /* the data pointer */
    std::shared_ptr<BTNode<T, KeyT>> left; /* left branch */
    std::shared_ptr<BTNode<T, KeyT>> right; /* right branch */
	std::shared_ptr<BTNode<T, KeyT>> parent; /* top branch */

    KeyT key; /* the key. Datatype must be comparable */

	/* Access functions */
	T Get() const;
	int Set(T ndata);
	int Set(std::shared_ptr<T> pndata);
	std::shared_ptr<BTNode<T, KeyT>> GetLeft() const;
	std::shared_ptr<BTNode<T, KeyT>> GetRight() const;
	std::shared_ptr<BTNode<T, KeyT>> GetParent() const;
	void SetLeft(std::shared_ptr<BTNode<T, KeyT>> btnode);
	void SetRight(std::shared_ptr<BTNode<T, KeyT>> btnode);
	void SetParent(std::shared_ptr<BTNode<T, KeyT>> btnode);
	KeyT GetKey() const;
	void SetKey(KeyT& newkey);

	/* Operator overloading */
	BTNode<T, KeyT>& operator= (const BTNode<T, KeyT>& btnode);
	BTNode<T, KeyT>& operator= (BTNode<T, KeyT>&& btnode) noexcept;

	/* Constructors and Destructors */
    BTNode();
    BTNode(T& ndata, KeyT& nkey);
	BTNode(std::shared_ptr<T> pndata, KeyT& nkey);
	BTNode(const BTNode<T, KeyT>& btnode);
	BTNode(BTNode<T, KeyT>&& btnode) noexcept;
	virtual ~BTNode() {;}
};



/* Binary tree main control class */
template <class T, class KeyT>
class BTree {
public:
	std::shared_ptr<BTNode<T, KeyT>> root_node; /* The root node */
	ULLONG nodes; /* Total number of nodes */

	bool IsLefty(std::shared_ptr<BTNode<T, KeyT>> n);
	std::shared_ptr<BTNode<T, KeyT>> Find(
		KeyT& k, std::shared_ptr<BTNode<T, KeyT>> r);
	std::shared_ptr<BTNode<T, KeyT>> FindMin(std::shared_ptr<BTNode<T, KeyT>> r);
	bool RemNode(
		std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r);
	bool InsNode(
		std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r);

	/* Access functions */
	T Get(KeyT& k) const;
	std::shared_ptr<BTNode<T, KeyT>> pGet(KeyT& k);
	int Set(T& ndata, KeyT& k);
	int Set(std::shared_ptr<T> pndata, KeyT& k);

	/* Manipulation */
	int Insert(T& ndata, KeyT& k);
	int Insert(std::shared_ptr<T> ndata, KeyT& k);
	int Remove(KeyT& k);

	/* Operator overloading */
	BTree<T, KeyT>& operator= (const BTree<T, KeyT>& bt);
	BTree<T, KeyT>& operator= (BTree<T, KeyT>&& bt) noexcept;

	/* Constructors and Destructors */
	BTree() : root_node(nullptr), nodes(0) {;}
	BTree(const BTree<T, KeyT>& bt);
	BTree(BTree<T, KeyT>&& bt) noexcept;
	virtual ~BTree() {;}
};




#include "btree.tcc"

#endif /* Include guard */
