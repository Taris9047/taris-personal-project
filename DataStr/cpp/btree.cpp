/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Binary Tree with arbitrary pointer type.

	Implementation file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#include "btree.hpp"

/*****************************************
  BTNode stuffs
******************************************/
/* The real definitions for BTNode */
struct BTree::btnode {
private:
  BTNode left;
  BTNode right;
  bt_data_t data;
  uint64_t key;

public:
  /* Some methods */
  BTNode GetLeft() const;
  void SetLeft(const BTNode& o) { left = o; }
  BTNode GetRight() const;
  void SetRight(const BTNode& o) { right = o; }
  uint64_t GetKey() const;
  void SetKey(uint64_t o_k) { key = o_k; }
  bt_data_t GetData() const;
  void SetData(bt_data_t d) { data = d; }

public:
  /* Constructors and destructors */
  btnode() : left(nullptr), right(nullptr), data(nullptr) {}
  btnode(const& btnode) : btnode()
  {
    left = btnode.left;
    right = btnode.right;
    data = btnode.data;
    key = btnode.key;
  }
  ~btnode() {}
};

/*****************************************
  BTree - Methods (Manipulation)
******************************************/
void BTree::Add(bt_data_t d, uint64_t key)
{
  #pragma omp critical
  {

  }
}


/*****************************************
  BTree - Access and utility methods
******************************************/


/*****************************************
  BTree - Constructors and Destructors
******************************************/
BTree::BTree() : root(nullptr), key_list(nullptr), size(0)
{}

BTree::BTree(const BTree& other) : BTree()
{

}
