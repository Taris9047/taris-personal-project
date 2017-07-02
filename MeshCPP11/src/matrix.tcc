/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T13:47:58-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T13:53:44-06:00
*/



/****************************************************

 Data structure c++11

 Simple Matrix Library

 Implementation file

 Written by Taylor Shin

 Mar. 9th 2017

*****************************************************/

#include <cassert>

/****************************************************
 Matrix::Access methods
*****************************************************/
template <class T>
T& Matrix<T>::At(const ULLONG& ir, const ULLONG& ic)
{
	assert(ir < rows || ic < cols);

	IndKey key(ir, ic);
	return BTData->Get(key);
}

template <class T>
int Matrix<T>::Set(const ULLONG& ir, const ULLONG& ic, T& d)
{
	IndKey key(ir, ic);

	if (ir >= rows || ic >= cols) {
		rows = ir+1;
		cols = ic+1;
		return BTData->Insert(d, key);
	}
	else return BTData->Set(d, key);
}

template <class T>
int Matrix<T>::Set(const ULLONG& ir, const ULLONG& ic,
	std::shared_ptr<T> pd)
{
	IndKey key(ir, ic);

	if (ir >= rows || ic >= cols) {
		rows = ir+1;
		cols = ic+1;
		return BTData->Insert(pd, key);
	}
	else return BTData->Set(pd, key);
}

template <class T>
bool Matrix<T>::Chk(const ULLONG& ir, const ULLONG& ic) const
{
	IndKey key(ir, ic);
	std::shared_ptr<BTNode<T, IndKey>> tmp = \
		BTData->pGet(key);

	if (tmp != nullptr) return true;
	else return false;
}


/****************************************************
 Matrix::Operators
*****************************************************/
template <class T>
T& Matrix<T>::operator () (const ULLONG& ir, const ULLONG& ic)
{
	return At(ir, ic);
}

template <class T>
Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
{
	Matrix<T> tmp(m);
	*this = std::move(tmp);
	return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator= (Matrix<T>&& m) noexcept
{
	BTData = nullptr;
	BTData = std::move(m.BTData);
	return *this;
}


/****************************************************
 Matrix::Manipulation methods
*****************************************************/



/****************************************************
 Matrix::Constructors and Destructors
*****************************************************/
/* Dummy constructor */
template <class T>
Matrix<T>::Matrix() :
	BTData(std::make_unique<BTree<T, IndKey>>()),
	rows(0),
	cols(0)
{
}

/* Initialization constructor */
template <class T>
Matrix<T>::Matrix(
	const ULLONG size_r, const ULLONG size_c, const T& init_data) : Matrix()
{
	rows = size_r;
	cols = size_c;

	ULLONG i, j;
	T tmp_data;
	IndKey tmp_key;

	for (i=0; i<rows; ++i) {
		for (j=0; j<cols; ++j) {
			tmp_data = init_data;
			tmp_key = IndKey(i,j);
			this->BTData->Insert(tmp_data, tmp_key);
		}
	}
}

/* Copy constructor */
template <class T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
	rows = m.Rows();
	cols = m.Cols();

	BTree<T, IndKey>& tmp = *m.BTData;
	BTData = std::make_unique<BTree<T, IndKey>>(tmp);
}

template <class T>
Matrix<T>::Matrix(Matrix<T>&& m) noexcept
{
	rows = m.Rows();
	cols = m.Cols();

	BTData = std::move(m.BTData);
}

/* Destructor */
template <class T>
Matrix<T>::~Matrix()
{
	BTData = nullptr;
}
