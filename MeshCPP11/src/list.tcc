/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:17:29-06:00
*/



/****************************************************

 Data structure c++11

 Linked list implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

/****************************************************
 LNode::Manipulation methods
*****************************************************/

template <class T>
void LNode<T>::SetNext(std::shared_ptr<LNode<T>> ln)
{
	if (ln) {
		this->next = std::move(ln);
		this->next->prev = this->shared_from_this();
	}
	else {
		if (this->next->prev) this->next->prev = nullptr;
		this->next = nullptr;
	}
}

template <class T>
void LNode<T>::SetPrev(std::shared_ptr<LNode<T>> ln)
{
	if (ln) {
		this->prev = std::move(ln);
		this->prev->next = this->shared_from_this();
	}
	else {
		if (this->prev->next) this->prev->next = nullptr;
		this->prev = nullptr;
	}
}

/****************************************************
 LNode::Access methods
*****************************************************/
template <class T>
T& LNode<T>::Get() const
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
std::shared_ptr<LNode<T>> LNode<T>::GetNext() const
{
	if (next) return next;
	else return nullptr;
}

template <class T>
std::shared_ptr<LNode<T>> LNode<T>::GetPrev() const
{
	if (prev) return prev;
	else return nullptr;
}

/****************************************************
 LNode::Operators
*****************************************************/
template <class T>
LNode<T>& LNode<T>::operator= (const LNode<T>& ln)
{
	LNode<T> tmp(ln);
	*this = std::move(tmp);
	return *this;
}

template <class T>
LNode<T>& LNode<T>::operator= (LNode<T>&& ln) noexcept
{
	data = std::move(ln.data);
	next = std::move(ln.next);
	prev = std::move(ln.prev);

	return *this;
}


/****************************************************
 LNode::Constructors and Destructors
*****************************************************/
template <class T>
LNode<T>::LNode(const LNode<T>& ln)
{
	T& tmp_data = *ln.data;
	LNode<T>& tmp_next = *ln.next;
	LNode<T>& tmp_prev = *ln.prev;

	data = std::make_shared<T>(tmp_data);
	next = std::make_shared<LNode<T>>(tmp_next);
	prev = std::make_shared<LNode<T>>(tmp_prev);
}

template <class T>
LNode<T>::LNode(LNode<T>&& ln) noexcept
{
	data = std::move(ln.data);
	next = std::move(ln.next);
	prev = std::move(ln.prev);
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
	std::shared_ptr<LNode<T>> new_node = \
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
T& List<T>::Pop()
{
	assert(root_node);

	T& stuff = root_node->Get();

	std::shared_ptr<LNode<T>> tmp = root_node->next;
	root_node->next = nullptr;
	root_node = std::move(tmp);
	root_node->prev = nullptr;
	len--;

	return stuff;
}

// Reverse the linked list
template <class T>
int List<T>::Reverse()
{
	if (!root_node) return -1;

	std::shared_ptr<LNode<T>> t;
	std::shared_ptr<LNode<T>> t_tmp;

	t = std::move(root_node);
	while (true) {
		t_tmp = t->next;
		t->next = t->prev;
		t->prev = t_tmp;
		if (t_tmp) t = t_tmp;
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
T& List<T>::At(ULLONG index) const
{
	ULLONG i;
	std::shared_ptr<LNode<T>> tmp;

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
List<T>::List(const List& l)
{
	root_node = l->GetL();
	len = l->Len();
}

template<class T>
List<T>::~List()
{
}
