/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Binary Tree with arbitrary pointer type.

	Header file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#ifndef MATRIX_CUDA_BINARY_TREE_HEADER
#define MATRIX_CUDA_BINARY_TREE_HEADER

#if defined(_OPENMP)
#  include <omp.h>
#endif

#include <memory>
#include <vector>
#include <cstdlib>
#include <cstdint>

#include "list.hpp"

/* data type */
typedef void* bt_data_t;
typedef uint64_t bt_key_t;

/* Binary tree class */
class BTree
{
public:
  /* Constructors and destructors */
  BTree();
  BTree(const BTree&);
  BTree& operator=(const BTree&);
  ~BTree() noexcept;

  /* Node struct */
  struct btnode;
  typedef btnode* BTNode;

  /* Manipulation methods */
  void Add(bt_data_t d, bt_key_t key);
  void Insert(bt_data_t d, bt_key_t key);
  void Remove(bt_key_t key);

  /* Access methods */
  bt_data_t Get(bt_key_t key);
  bt_data_t operator() (bt_key_t key);

  /* Some utils */
  bool IsEmpty();
  bt_key_t Size();

private:
  BTNode root;
  uint64_t size;
  List* node_list;

  bool Exists(bt_data_t);
  bt_data_t Find(uint64_t key);

  void destroy(BTNode);
};


#endif /* Include guard */
