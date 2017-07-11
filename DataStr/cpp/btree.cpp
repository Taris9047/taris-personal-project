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
  bt_key_t key;

public:
  /* Some methods */
  BTNode GetLeft() const { return left; }
  void SetLeft(const BTNode& o) { left = o; }
  BTNode GetRight() const { return right; }
  void SetRight(const BTNode& o) { right = o; }
  bt_key_t GetKey() const { return key; }
  void SetKey(bt_key_t o_k) { key = o_k; }
  bt_data_t GetData() const { return data; }
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
void BTree::Add(bt_data_t d, bt_key_t key)
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

  #pragma omp critical
  {
  size++;
  }
  // bt_key_t* p_key = new bt_key_t(key);
  // key_list->Push(p_key);

}

/* Same as Add... alias */
void BTree::Insert(bt_data_t d, bt_key_t key)
{
  Add(d, key);
}

/*****************************************
  BTree - Access and utility methods
******************************************/
bool BTree::IsEmpty()
{
  if (!root) return true;
  else return false;
}

uint64_t BTree::Size()
{
  return size;
}

/*****************************************
  BTree - Private methods
******************************************/

/* try to find given data (in fact, pointer addr.) */
/*
  Reference:
  http://www.geeksforgeeks.org/iterative-preorder-traversal/
*/
bool BTree::Exists(bt_data_t data)
{
  /* Empty the node_list */
  if (!node_list->IsEmpty())
    node_list->DeleteNodes();

  /* Let's start it */
  node_list->Push(root);

  BTNode tmp;
  while (!node_list->IsEmpty()) {

    tmp = (BTNode)node_list->At(0);
    if (data == tmp->GetData()) return true;
    node_list->Pop();

    if (tmp->GetRight()) node_list->Push(tmp->GetRight());
    if (tmp->GetLeft()) node_list->Push(tmp->GetLeft());

  } /* while (!node_list->IsEmpty()) */

  return false;
}

/* Find a node with key */
bt_data_t BTree::Find(bt_key_t key)
{
  if (IsEmpty()) return nullptr;

  /* Maybe we need to implement some validation routine for given key here.
    If given key doesn't exist in this tree, botch it!! */

  /* Currently, finding routine is a mere single core */
  BTNode tmp_btnode = root;
  while (true) {
    if (key == tmp_btnode->GetKey()) return tmp_btnode->GetData();
    else if (key < tmp_btnode->GetKey())
      tmp_btnode = tmp_btnode->GetLeft();
    else
      tmp_btnode = tmp_btnode->GetRight();
  }

  /* None shall pass here !! */
  return nullptr;
}

/* Destroys all the nodes recursively */
void BTree::destroy(BTNode btn)
{
  if (!btn) return;

  if (!node_list->IsEmpty())
    node_list->DeleteNodes();

  BTNode tmp;
  node_list->Push(root);

  while (!node_list->IsEmpty()) {

    tmp = (BTNode)node_list->At(0);

    if (tmp->GetLeft()) node_list->Push(tmp->GetLeft());
    if (tmp->GetRight()) node_list->Push(tmp->GetRight());

    delete tmp;

  } /* while (!node_list->IsEmpty()) */

}

/*****************************************
  BTree - Constructors and Destructors
******************************************/
BTree::BTree() : root(nullptr), node_list(new List), size(0)
{
}

BTree::BTree(const BTree& other) : BTree()
{
}

BTree::~BTree()
{
  destroy(root);
  delete node_list;
}
