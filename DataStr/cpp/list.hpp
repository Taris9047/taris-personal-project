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

#include <cstdint>
#include <memory>

/* list_data_t... void* */
typedef void* list_data_t;

/* List node definition */
struct lnode;
typedef lnode* LNode;

/* Class definition */
class List {
public:
	/* Constructors and destructors */
	List();
	List(const List&);
	List(List&&) noexcept;
	List& operator= (const List&);
  List& operator= (List&&) noexcept;
	~List() noexcept;

	/* Manipulation methods */
	void Push(list_data_t d);
	void Push_back(list_data_t d);
	list_data_t Pop();
	list_data_t Pop_back();
	void Reverse();

	void DeleteValues(); /* Delete data! An intrusive method. Use with caution!! */
	void DeleteNodes(); /* Delete all nodes. clean up!! */

	/* Access methods */
	uint64_t Len() const;
	list_data_t At(uint64_t ind) const;
	bool IsEmpty() const;

private:
	LNode root;
	LNode last;
	LNode cursor;

	uint64_t length;
	uint64_t cursor_loc;
};


#endif /* Include guard */
