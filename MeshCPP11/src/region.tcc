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
template <class T, class KeyT>
int Region<T, KeyT>::make_nodes()
{
	if (!root_node)
		root_node = \
			std::make_shared<MNode<T, KeyT>>();

	if (!MNodes) {
		MNodes = std::unique_ptr<Matrix<std::shared_ptr<MNode<T, KeyT>>>>();
		MNodes->Set(0, 0, root_node);
	}

	if (rows == 1 && cols == 1) return 0;

	ULLONG i, j;
	std::shared_ptr<MNode<T, KeyT>> tmp = root_node;
	std::shared_ptr<MNode<T, KeyT>> new_node;

	/* Make first row */
	for (i=1; i<rows; ++i) {
		new_node = std::make_shared<MNode<T, KeyT>>();
		tmp->SetDN(new_node);
		MNodes->Set(i, 0, new_node);
		tmp = tmp->dn;
	}
	tmp = root_node;
	/* then make entire row */
	for (i=0; i<rows; ++i) {
		for (j=1; j<cols; ++j) {
			new_node = std::make_shared<MNode<T, KeyT>>();
			tmp->SetRH(new_node);
			MNodes->Set(i, j, new_node);
			tmp = tmp->rh;
		}
	}

	return i*j;
}


/****************************************************
 Region::Constructors and Destructors
*****************************************************/
template <class T, class KeyT>
Region<T, KeyT>::Region() : \
	MNodes(nullptr),
	root_node(nullptr),
	rows(1), cols(1)
{
}

template <class T, class KeyT>
Region<T, KeyT>::Region::Region(
	const ULLONG size_r, const ULLONG size_c) : \
	Region()
{
	rows = size_r; cols = size_c;
	make_nodes();
}

template <class T, class KeyT>
Region<T, KeyT>::Region(const Region<T, KeyT>& r)
{
	MNodes = r.GetNodes();
	root_node = r.GetRootNode();
	rows = r.Rows();
	cols = r.Cols();
}

template <class T, class KeyT>
Region<T, KeyT>::~Region()
{
}
