/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T13:32:59-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T13:33:37-06:00
*/



/****************************************************

 Data structure c++11

 Region implementation file

 Written by Taylor Shin

 Mar. 9th 2017

*****************************************************/

#include "utils.hpp"

/****************************************************
 Region::Private methods
*****************************************************/
template <class T>
int Region<T>::make_nodes()
{
	if (!root_node)
		root_node = std::make_shared<MNode<T, IndKey>>();

	if (!MNodes) {
		MNodes = std::make_unique<Matrix< std::shared_ptr<MNode<T, IndKey>>>>();
		MNodes->Set(0, 0, root_node);
	}

	if (rows == 1 && cols == 1) return 0;

	ULLONG i, j;
	std::shared_ptr<MNode<T, IndKey>> tmp = root_node;
	std::shared_ptr<MNode<T, IndKey>> tmp_h;
	std::shared_ptr<MNode<T, IndKey>> new_node;

	/* Make first row */
	for (i=1; i<rows; ++i) {
		new_node = std::make_shared<MNode<T, IndKey>>();
		tmp->SetDN(new_node);
		MNodes->Set(i, 0, new_node);
		tmp = tmp->dn;
	}
	tmp = root_node;
	tmp_h = tmp;
	/* then make entire row */
	for (i=0; i<rows; ++i) {
		tmp_h = tmp;
		for (j=1; j<cols; ++j) {
			new_node = std::make_shared<MNode<T, IndKey>>();
			tmp->SetRH(new_node);
			MNodes->Set(i, j, new_node);
			tmp = tmp->rh;
		}
		tmp = tmp_h->dn;
	}

	return i*j;
}

template <class T>
int Region<T>::delete_nodes()
{
	if (!root_node) return 1;

	if (rows == 1 && cols == 1) root_node = nullptr;

	std::shared_ptr<MNode<T, IndKey>> tmp = root_node;

	return del_node(tmp);
}

template <class T>
int Region<T>::del_node(std::shared_ptr<MNode<T, IndKey>> n)
{
	if (n->rh) del_node(n->rh);
	if (n->rd) del_node(n->rd);
	if (n->dn) del_node(n->dn);

	n->rh = nullptr;
	n->rd = nullptr;
	n->dn = nullptr;
	n->lh = nullptr;
	n->lu = nullptr;
	n->up = nullptr;

	n = nullptr;

	return 0;
}

/****************************************************
 Region::Access
*****************************************************/
template <class T>
T& Region<T>::At(const ULLONG& r, const ULLONG& c)
{
	/* Let's take advantage of Matrix */
	return MNodes->At(r, c)->Get();
}


/****************************************************
 Region::Manipulation
*****************************************************/
/* 2D data assignment. Make sure your data's dimension matches class */
template <class T>
void Region<T>::AssignData(T** data)
{
	assert(data);

	ULLONG i, j;

	std::shared_ptr<MNode<T, IndKey>> tmp;

	for (i=0; i<rows; ++i) {
		for (j=0; j<cols; ++j) {
			IndKey key(i, j);
			tmp = std::make_shared<MNode<T, IndKey>>(data[i][j], key);
			MNodes->Set(i, j, tmp);
		}
	}
}

template <class T>
void Region<T>::AssignData(std::vector<std::vector<T>> data)
{
	ULLONG i, j;

	std::shared_ptr<MNode<T, IndKey>> tmp;

	for (i=0; i<rows; ++i) {
		for (j=0; j<cols; ++j) {
			IndKey key(i, j);
			tmp = std::make_shared<MNode<T, IndKey>>(data[i][j], key);
			MNodes->Set(i, j, tmp);
		}
	}
}

template <class T>
void Region<T>::AssignData(std::vector<std::vector<std::shared_ptr<T>>> pdata)
{
	ULLONG i, j;

	std::shared_ptr<MNode<T, IndKey>> tmp;

	for (i=0; i<rows; ++i) {
		for (j=0; j<cols; ++j) {
			IndKey key(i, j);
			tmp = std::make_shared<MNode<T, IndKey>>(*pdata[i][j], key);
			MNodes->Set(i, j, tmp);
		}
	}
}


/****************************************************
 Region::Operator Overloading
*****************************************************/
template <class T>
Region<T>& Region<T>::operator= (const Region<T>& r)
{
	Region<T> tmp(r);
	*this = std::move(tmp);
	return *this;
}

template <class T>
Region<T>& Region<T>::operator= (Region<T>&& r) noexcept
{
	MNodes = std::move(r.MNodes);
	root_node = std::move(r.root_node);
	rows = r.Rows();
	cols = r.Cols();

	return *this;
}

template <class T>
T& Region<T>::operator() (const ULLONG& r, const ULLONG& c)
{
	return At(r, c);
}

/****************************************************
 Region::Constructors and Destructors
*****************************************************/
template <class T>
Region<T>::Region() : \
	root_node(nullptr),
	rows(1), cols(1)
{
}

template <class T>
Region<T>::Region::Region(
	const ULLONG& size_r, const ULLONG& size_c) : \
	Region()
{
	rows = size_r; cols = size_c;
	make_nodes();
}

template <class T>
Region<T>::Region(const Region<T>& r)
{
	Matrix< std::shared_ptr<MNode<T, IndKey>> >& MN_tmp = *r.GetNodes();
	MNode<T, IndKey>& rn_tmp = *r.GetRootNode();

	MNodes = std::make_unique<Matrix< std::shared_ptr<MNode<T, IndKey>> >>(MN_tmp);
	root_node = std::make_shared<MNode<T, IndKey>>(rn_tmp);

	rows = r.Rows();
	cols = r.Cols();
}

template <class T>
Region<T>::Region(Region<T>&& r) noexcept
{
	MNodes = std::move(r.MNodes);
	root_node = std::move(r.root_node);

	rows = r.Rows();
	cols = r.Cols();
}

template <class T>
Region<T>::~Region()
{
	delete_nodes();
	MNodes = nullptr;
}
