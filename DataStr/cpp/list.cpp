/*****************************************

  Another attempt to implement
  a data structure with C++11 (STL)

  Linked list

  Implementation file.

  Written by Taylor Shin
  June 22th 2017

******************************************/

#include <cstdlib>

#include "list.hpp"

/********************************************
  Stuffs for LNode
*********************************************/

/* List Node re-definition */
struct List::lnode {
private:
  LNode next;
  LNode prev;
  list_data_t value;

public:
  /* Constructors and destructor s*/
  lnode(): next(nullptr), prev(nullptr), value(nullptr) {}
  lnode(const lnode& ln) : \
    next(ln.Next()), prev(ln.Prev()), value(ln.Value())
  {}
  lnode(lnode&& ln) noexcept : lnode(ln)
  { ln.SetValue(nullptr); }
  lnode& operator= (const lnode& ln)
  {
    lnode tmp(ln);
    *this = std::move(tmp);
    return *this;
  }
  lnode& operator= (lnode&& ln) noexcept
  {
    value = ln.Value();
    ln.SetValue(nullptr);
    return *this;
  }
  ~lnode() noexcept {}

  /* Access methods */
  LNode Next() const { return next; }
  void SetNext(LNode ln) { next = ln; }
  LNode Prev() const { return prev; }
  void SetPrev(LNode ln) { prev = ln; }
  list_data_t Value() const { return value; }
  void SetValue(list_data_t val) { value = val; }
};


/********************************************
  List class manipulation methods ...
*********************************************/
/* Push new data */
void List::Push(list_data_t d)
{
  LNode tmp_node = new lnode();
  tmp_node->SetValue(d);

  if (root) {
    tmp_node->SetNext(root);
    root->SetPrev(tmp_node);
  }
  root = tmp_node;
  ++length;

  if (!last) last = root;
  if (!cursor) {
    cursor = root;
    cursor_loc = 0;
  }
  else ++cursor_loc;
}

/* Push new data into the last */
void List::Push_back(list_data_t d)
{
  LNode tmp_node = new lnode();
  tmp_node->SetValue(d);

  if (last) {
    tmp_node->SetPrev(last);
    last->SetNext(tmp_node);
    last = tmp_node;
  }
  ++length;

  if (!root) root = last;
  if (!cursor) {
    cursor = root;
    cursor_loc = 0;
  }
}

/* Pop first node */
list_data_t List::Pop()
{
  if (!length) return nullptr;

  list_data_t data = root->Value();
  LNode tmp_next = root->Next();

  if (!cursor_loc) cursor = tmp_next;
  if (last == root) last = tmp_next;
  delete root;
  root = tmp_next;

  cursor_loc--;

  return data;
}

/* Reverse it */
void List::Reverse()
{
  if (this->IsEmpty()) return;
  if (length == 1) return;

  LNode tn, tmp_node, tmp_node_next;

  if (length == 2 || length == 3) {
    tmp_node = root;
    last = root;
    root = tmp_node;
    this->ResetCursor();
    return;
  }

  uint64_t i;
  tmp_node = root;
  tmp_node_next = tmp_node->Next();
  for (i=0; i<length; ++i) {
    tn = tmp_node->Next();
    tmp_node->SetNext(tmp_node->Prev());
    tmp_node->SetPrev(tn);

    tmp_node = tmp_node_next;
    if (tmp_node->Next()) tmp_node_next = tmp_node->Next();
    else tmp_node_next = nullptr;
  }

  tn = root;
  root = last;
  last = tn;
  this->ResetCursor();

  return;
}

/* Delete all the values... or at least trying */
void List::DeleteValues()
{
  if (!length) return;

  LNode tmp_lnode = root;
  while (tmp_lnode) {
    if (tmp_lnode->Value())
      free(tmp_lnode->Value());
    tmp_lnode = tmp_lnode->Next();
  }
}

/* Delete all the nodes */
void List::DeleteNodes()
{
  if (!length) return;

  uint64_t i;
  LNode tmp_lnode = root, tmp_lnode_curr;
  for (i=0; i<length; ++i) {
    tmp_lnode_curr = tmp_lnode;
    tmp_lnode = tmp_lnode->Next();
    delete tmp_lnode_curr;
  }
  root = nullptr;
  last = nullptr;
  cursor = nullptr;
}

/********************************************
  List class access methods ...
*********************************************/
/* Return legnth */
uint64_t List::Len() const
{
  return length;
}

/* Access a certain index */
list_data_t List::At(uint64_t ind)
{
  if (ind+1 > length) {
    std::cerr << "List.At: Index out of range!!" << std::endl;
    throw std::exception();
  }

  /* Some shortcuts */
  if (ind == cursor_loc) return cursor->Value();
  if (ind == 0) return root->Value();
  if (ind == length-1) return last->Value();

  /* Let's check up distances */
  uint64_t d_root, d_last, d_cursor;
  d_root = ind;
  d_last = length-1 - ind;
  d_cursor = (cursor_loc>ind)?(cursor_loc-ind):(ind-cursor_loc);
  std::array<uint64_t, 3> dist_ary = {d_root, d_last, d_cursor};
  std::sort(dist_ary.begin(), dist_ary.end());

  LNode tmp_node;
  uint64_t i;
  if (dist_ary[0] == d_root) {
    tmp_node = root;
    for (i=0; i<d_root; ++i) tmp_node = tmp_node->Next();
  } /* if (dist_ary[0] == d_root) */
  else if (dist_ary[0] == d_last) {
    tmp_node = last;
    for (i=0; i<d_last; ++i) tmp_node = tmp_node->Prev();
  } /* else if (dist_ary[0] == d_last) */
  else if (dist_ary[0] == d_cursor) {
    tmp_node = cursor;
    if (cursor_loc > ind)
      for (i=0; i<d_cursor; ++i) tmp_node = tmp_node->Prev();
    else
      for (i=0; i<d_cursor; ++i) tmp_node = tmp_node->Next();
  } /* else if (dist_ary[0] == d_cursor) */

  return tmp_node->Value();
}

/* Same but const version. Slower */
list_data_t List::At(uint64_t ind) const
{
  if (ind+1 > length) {
    std::cerr << "List.At const: Index out of range!!" << std::endl;
    throw std::exception();
  }

  /* Some shortcuts */
  if (ind == cursor_loc) return cursor->Value();
  if (ind == 0) return root->Value();
  if (ind == length-1) return last->Value();

  uint64_t i;
  LNode tmp_node = root;
  for (i=0; i<ind+1; ++i)
    tmp_node = tmp_node->Next();

  return tmp_node->Value();
}

/* Access with sequential pattern */
list_data_t List::AtSeq(uint64_t ind)
{
  if (ind+1 > length) {
    std::cerr << "List.AtSeq: Index out of range!!" << std::endl;
    throw std::exception();
  }

  /* Some shortcuts */
  if (ind == cursor_loc) return cursor->Value();
  if (ind == 0) return root->Value();
  if (ind == length-1) return last->Value();
  if (cursor_loc == ind-1) {
    cursor = cursor->Next();
    cursor_loc++;
    return cursor->Value();
  }

  uint64_t i;
  uint64_t d_root = ind;
  uint64_t d_cursor = (cursor_loc>ind)?(cursor_loc-ind):(ind-cursor_loc);
  LNode tmp_node;

  if (d_root < d_cursor) {
    tmp_node = root;
    for (i=0; i<d_root; ++i) tmp_node = tmp_node->Next();
  }
  else if (d_root > d_cursor) {
    tmp_node = cursor;
    if (cursor_loc > ind)
      for (i=0; i<d_cursor; ++i) tmp_node = tmp_node->Prev();
    else
      for (i=0; i<d_cursor; ++i) tmp_node = tmp_node->Next();
  }

  cursor = tmp_node;
  cursor_loc = ind;

  return tmp_node->Value();
}


/* Check empty list */
bool List::IsEmpty() const
{
  if (!this->root && !this->last) return true;
  else return false;
}

/* Reset Cursor */
void List::ResetCursor()
{
  cursor = root;
  cursor_loc = 0;
}


/********************************************
  List class constructors and destructors
*********************************************/
/* Default constructor */
List::List() : \
  root(nullptr), \
  last(nullptr), \
  cursor(nullptr), \
  length(0), \
  cursor_loc(0)
{
}

/* Copy constructor */
List::List(const List& lst) : List()
{
  length = lst.Len();
  if (lst.IsEmpty()) return;

  uint64_t i;
  LNode tmp_lnode;
  for (i=0; i<length; ++i) {
    tmp_lnode = new lnode(); /* Make a new node in the heap */
    /*
      Note that we don't copy the data. Just referencing them but we also
      need to keep the list doesn't touch the original data.
    */
    tmp_lnode->SetValue(lst.At(i));
    if (i==0) {
      root = tmp_lnode;
      last = tmp_lnode;
      cursor = tmp_lnode;
    } /* if (i==0) */
    else {
      tmp_lnode->SetPrev(last);
      last->SetNext(tmp_lnode);
      last = tmp_lnode;
    } /* if (i==0) else */
  } /* for (i=0; i<length; ++i) */
}

/* Copy operator */
List& List::operator= (const List& other)
{
  List tmp(other);
  *this = std::move(tmp);
  return *this;
}

/* Destructor */
List::~List() noexcept
{
  this->DeleteNodes();
}
