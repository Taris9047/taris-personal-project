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
class BTNode {
private:
    std::shared_ptr<T> data; /* the data pointer */
    std::shared_ptr<BTNode<T, KeyT>> left; /* left branch */
    std::shared_ptr<BTNode<T, KeyT>> right; /* right branch */

    KeyT key; /* the key. Datatype must be comparable */

public:
	/* Access functions */
	T Get() const;
	void Set(T ndata);
	std::shared_ptr<BTNode<T, KeyT>> GetLeft() const;
	std::shared_ptr<BTNode<T, KeyT>> GetRight() const;
	void SetLeft(std::shared_ptr<BTNode<T, KeyT>>& btnode);
	void SetRight(std::shared_ptr<BTNode<T, KeyT>>& btnode);
	KeyT GetKey() const;
	void SetKey(KeyT& newkey);

	/* Constructors and Destructors */
    BTNode() : data(nullptr), left(nullptr), right(nullptr) {;}
    BTNode(T ndata, KeyT& nkey);
	BTNode(BTNode<T, KeyT>& btnode);
	virtual ~BTNode() {;}
};



/* Binary tree main control class */
template <class T, class KeyT>
class BTree {
private:
	std::shared_ptr<BTNode<T, KeyT>> root_node; /* The root node */
	ULLONG nodes; /* Total number of nodes */

	std::shared_ptr<BTNode<T, KeyT>> Find(KeyT k);

public:
	/* Access functions */
	T Get(KeyT k) const;
	void Set(T& ndata);

	/* Manipulation */
	void Insert(T& ndata, KeyT k);
	void Remove(KeyT k);

	/* Constructors and Destructors */
	BTree() : root_node(nullptr), nodes(0);
	BTree(BTree& bt);
};




#include "btree.tcc"

#endif /* Include guard */
