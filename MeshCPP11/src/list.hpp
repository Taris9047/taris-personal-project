/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:17:19-06:00
*/



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

#include "utils.hpp"

/* The linked list node class */
template <class T> /* datatype */
class LNode : public std::enable_shared_from_this<LNode<T>>
{
public:
    std::shared_ptr<T> data; /* The data pointer */
    std::shared_ptr<LNode<T>> next; /* next node */
    std::shared_ptr<LNode<T>> prev; /* previous node */

    T Get() const;
    int Set(std::shared_ptr<T>& ndata);
    std::shared_ptr<LNode<T>> GetNext() const;
    std::shared_ptr<LNode<T>> GetPrev() const;
    void SetNext(std::shared_ptr<LNode<T>> ln);
    void SetPrev(std::shared_ptr<LNode<T>> ln);

    LNode() : data(nullptr), next(nullptr), prev(nullptr) {;}
    LNode(T& d) : LNode() { data = std::make_shared<T>(d); }
    LNode(LNode& ln);
    ~LNode();
};

/* The linked list control class */
template <class T>
class List {
private:
    std::shared_ptr<LNode<T>> root_node;
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
    virtual ~List();
};

#include "list.tcc"

#endif /* Include guard */
