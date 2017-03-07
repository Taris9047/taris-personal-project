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

#include "utils.h"

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
	void Set(T ndata);
	std::shared_ptr<BTNode<T, KeyT>> GetLeft() const;
	std::shared_ptr<BTNode<T, KeyT>> GetRight() const;
	std::shared_ptr<BTNode<T, KeyT>> GetParent() const;
	void SetLeft(std::shared_ptr<BTNode<T, KeyT>> btnode);
	void SetRight(std::shared_ptr<BTNode<T, KeyT>> btnode);
	void SetParent(std::shared_ptr<BTNode<T, KeyT>> btnode);
	KeyT GetKey() const;
	void SetKey(KeyT& newkey);

	/* Constructors and Destructors */
    BTNode() : data(nullptr), left(nullptr), right(nullptr) {;}
    BTNode(T& ndata, KeyT& nkey);
	BTNode(BTNode<T, KeyT>& btnode);
	virtual ~BTNode() {;}
};



/* Binary tree main control class */
template <class T, class KeyT>
class BTree {
private:
	std::shared_ptr<BTNode<T, KeyT>> root_node; /* The root node */
	ULLONG nodes; /* Total number of nodes */

	bool IsLefty(std::shared_ptr<BTNode<T, KeyT>> n);
	std::shared_ptr<BTNode<T, KeyT>> Find(
		KeyT& k, std::shared_ptr<BTNode<T, KeyT>> r);
	std::shared_ptr<BTNode<T, KeyT>> FindMin(std::shared_ptr<BTNode<T, KeyT>> r);
	std::shared_ptr<BTNode<T, KeyT>> Traverse(
		std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r);
	bool RemNode(
		std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r);
	bool InsNode(
		std::shared_ptr<BTNode<T, KeyT>> bt, std::shared_ptr<BTNode<T, KeyT>> r);

public:
	/* Access functions */
	T Get(KeyT& k) const;
	void Set(T& ndata, KeyT& k);

	/* Manipulation */
	void Insert(T& ndata, KeyT& k);
	void Remove(KeyT& k);

	/* Constructors and Destructors */
	BTree() : root_node(nullptr), nodes(0) {;}
	BTree(BTree<T, KeyT>& bt);
};




#include "btree.tcc"

#endif /* Include guard */
