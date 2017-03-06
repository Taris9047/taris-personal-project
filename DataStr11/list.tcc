/****************************************************

 Data structure c++11

 Linked list implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

//#include <swap>

//#include "list.h"

/****************************************************
 LNode::Manipulation methods
*****************************************************/

template <class T>
int LNode<T>::SetNext(std::shared_ptr<LNode<T>> ln)
{
	next = std::move(ln);
	return 0;
}

template <class T>
int LNode<T>::SetPrev(std::shared_ptr<LNode<T>> ln)
{
	prev = std::move(ln);
	return 0;
}

/****************************************************
 LNode::Access methods
*****************************************************/
template <class T>
T LNode<T>::Get() const
{
	return *data;
}

template <class T>
int LNode<T>::Set(std::shared_ptr<T>& ndata)
{
	data = ndata;
	return 0;
}

template <class T>
spLNode<T> LNode<T>::GetNext() const
{
	if (next) return next;
	else return nullptr;
}

template <class T>
spLNode<T> LNode<T>::GetPrev() const
{
	if (prev) return prev;
	else return nullptr;
}

/****************************************************
 LNode::Constructors and Destructors
*****************************************************/
template <class T>
LNode<T>::LNode(LNode& ln) : LNode<T>::LNode()
{
	data = ln.Get();
	next = ln.GetNext();
	prev = ln.GetPrev();
}

template <class T>
LNode<T>::~LNode()
{
	data = nullptr;
	next = nullptr;
	prev = nullptr;
}
















/****************************************************
 List::Manipulation methods
*****************************************************/
// Push
template <class T>
int List<T>::Push(T& new_stuff)
{
	//std::shared_ptr<T> sp_new_stuff = std::make_shared<T>(new_stuff);
	spLNode<T> new_node = \
		std::make_unique<LNode<T>>(new_stuff);

	if (this->IsEmpty()) {
		root_node = std::move(new_node);
	}
	else {
		new_node->SetNext(root_node);
		root_node->SetPrev(new_node);
		root_node = std::move(new_node);
	}
	len++;

	return 0;
}

// Pop
template <class T>
T List<T>::Pop()
{
	assert(root_node);

	T stuff = root_node->Get();

	spLNode<T> tmp = root_node->GetNext();
	root_node->SetNext(nullptr);
	root_node = std::move(tmp);
	root_node->SetPrev(nullptr);
	len--;

	return stuff;
}

// Reverse the linked list
template <class T>
int List<T>::Reverse()
{
	if (!root_node) return -1;

	spLNode<T> t, t_tmp;

	t = std::move(root_node);
	while (true) {
		t_tmp = std::move(t->GetNext());
		t->SetNext(t->GetPrev());
		t->SetPrev(t_tmp);
		if (t_tmp) t = std::move(t_tmp);
		else break;
	}

	root_node = std::move(t);

	return 0;
}

/****************************************************
 List::Access methods
*****************************************************/
// Access by index
template <class T>
T List<T>::At(ULLONG index) const
{
	ULLONG i;
	spLNode<T> tmp;

	tmp = std::move(root_node);
	for (i=0; i<index; ++i) {
		tmp = tmp->GetNext();
	}
	return tmp->Get();
}

// Is Empty?
template <class T>
bool List<T>::IsEmpty() const
{
	if (root_node == nullptr) return true;
	else return false;
}

// List?
template <class T>
LNode<T>& List<T>::GetL() const
{
	return root_node;
}

// Length?
template <class T>
ULLONG List<T>::Len() const
{
	return len;
}


/****************************************************
 List::Constructors and Destructors
*****************************************************/
template<class T>
List<T>::List() : root_node(nullptr), len(0)
{}

template<class T>
List<T>::List(List& l)
{
	root_node = l->GetL();
	len = l->Len();
}

template<class T>
List<T>::~List()
{
}
