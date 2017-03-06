/****************************************************

 Data structure c++11

 Linked list header file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

#ifndef CPP11_LIST_H
#define CPP11_LIST_H

#include <memory>
#include <cassert>

#include "utils.h"

/* The linked list node class */
template <class T> /* datatype */
class LNode {
private:
    std::shared_ptr<T> data; /* The data pointer */
    std::shared_ptr<LNode<T>> next; /* next node */
    std::shared_ptr<LNode<T>> prev; /* previous node */

public:
    T Get() const;
    int Set(std::shared_ptr<T>& ndata);
    std::shared_ptr<LNode<T>> GetNext() const;
    std::shared_ptr<LNode<T>> GetPrev() const;
    int SetNext(std::shared_ptr<LNode<T>> ln);
    int SetPrev(std::shared_ptr<LNode<T>> ln);

    LNode() : data(nullptr), next(nullptr), prev(nullptr) {;}
    LNode(T& d) : LNode() { data = std::make_shared<T>(d); }
    LNode(LNode& ln);
    ~LNode();
};
template <class T> using spLNode = std::shared_ptr<LNode<T>>;

/* The linked list control class */
template <class T>
class List {
private:
    spLNode<T> root_node;
    ULLONG len;

public:
    // Push, Pop, Access, Reverse methods
    int Push(T& new_stuff);
    T Pop();
    T At(ULLONG index) const;
    int Reverse();

    // Status check methods
    bool IsEmpty() const;
    LNode<T>& GetL() const;
    ULLONG Len() const;

    // Constructors and destructors
    List();
    List(List& l);
    ~List();
};

#include "list.tcc"

#endif /* Include guard */
