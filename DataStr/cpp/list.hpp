/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Linked list with arbitrary pointer type.

	Header file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_LIST_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_LIST_HEADER

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>

/* list_data_t... void* */
typedef void* list_data_t;

/* Class definition */
class List {
public:
	/* Constructors and destructors */
	List();
	List(const List&);
	List& operator= (const List&);
	~List() noexcept;

	/* Manipulation methods */
	void Push(list_data_t);
	void Push_back(list_data_t);
	list_data_t Pop();
	list_data_t Pop_back();
	void Reverse();

	void DeleteValues(); /* Delete data! An intrusive method. Use with caution!! */
	void DeleteNodes(); /* Delete all nodes. clean up!! */

	/* Access methods */
	uint64_t Len() const;
  /* An intelligent (?) element search method */
	list_data_t At(uint64_t);
  /* Not so intelligent element search - iterates from root... last resort */
  list_data_t At(uint64_t) const;
  /* Sequential element search */
  list_data_t AtSeq(uint64_t);
  /* Is the List empty? */
	bool IsEmpty() const;
  /* Reset cursor */
  void ResetCursor();

private:
  struct lnode;
  typedef lnode* LNode;

	LNode root;
	LNode last;
	LNode cursor;

	uint64_t length;
	uint64_t cursor_loc;
};


#endif /* Include guard */
