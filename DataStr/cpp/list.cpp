/*****************************************

  Another attempt to implement
  a data structure with C++11 (STL)

  Linked list

  Implementation file.

  Written by Taylor Shin
  June 22th 2017

******************************************/

#include "list.hpp"

/********************************************
  Stuffs for LNode
*********************************************/
/* List Node re-definition */
struct lnode {
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
  void SetNext(const LNode& ln) { next = ln->next; }
  LNode Prev() const { return prev; }
  void SetPrev(const LNode& ln) { next = ln->prev; }
  list_data_t Value() const { return value; }
  void SetValue(list_data_t val) { value = val; }
};





/********************************************
  List class methods ...
*********************************************/

/* Delete all the values... or at least trying */
void List::DeleteValues()
{
  if (!length) return;

  LNode tmp_lnode = root;
  while (tmp_lnode) {
    delete (void*)tmp_lnode->Value();
    tmp_lnode = tmp_lnode->Next();
  }
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
