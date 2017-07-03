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
  BTNode GetLeft() const { return left; };
  void SetLeft(const BTNode& o) { left = o; }
  BTNode GetRight() const { return right; };
  void SetRight(const BTNode& o) { right = o; }
  uint64_t GetKey() const { return key; };
  void SetKey(uint64_t o_k) { key = o_k; }
  bt_data_t GetData() const { return data; };
  void SetData(bt_data_t d) { data = d; }

public:
  /* Constructors and destructors */
  btnode() : left(nullptr), right(nullptr), data(nullptr) {}
  btnode(const btnode& other) : btnode()
  {
    left = other.left;
    right = other.right;
    data = other.data;
    key = other.key;
  }
  ~btnode() {}
};

/*****************************************
  BTree - Methods (Manipulation)
******************************************/
void BTree::Add(bt_data_t d, uint64_t key)
{
  BTNode new_node = new btnode();
  new_node->SetData(d);
  new_node->SetKey(key);

  BTNode tmp_node;
  if (!root)
    root = new_node;
  else {
    tmp_node = root;
    while(true) {
      if (tmp_node->GetKey() < key) {
        if (!tmp_node->GetLeft()) {
          #pragma omp critical
          tmp_node->SetLeft(new_node);
          break;
        }
        else {
          #pragma omp critical
          tmp_node = tmp_node->GetLeft();
        }
      }
      else if (tmp_node->GetKey() == key) {
        #pragma omp critical
        tmp_node->SetData(d);
        delete new_node;
        break;
      }
      else {
        if (!tmp_node->GetRight()) {
          #pragma omp critical
          tmp_node->SetRight(new_node);
          break;
        }
        else {
          #pragma omp critical
          tmp_node = tmp_node->GetRight();
        }
      }
    } /* while(true) */
  } /* if (!this->root) */
}


/*****************************************
  BTree - Access and utility methods
******************************************/

/*****************************************
  BTree - Private methods
******************************************/

/* Find a node with key */
bt_data_t Find(uint64_t key)
{

}

/*****************************************
  BTree - Constructors and Destructors
******************************************/
BTree::BTree() : root(nullptr), key_list(nullptr), size(0)
{}

BTree::BTree(const BTree& other) : BTree()
{

}
