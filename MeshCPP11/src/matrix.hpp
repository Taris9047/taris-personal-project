/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T13:47:58-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T13:53:41-06:00
*/



/****************************************************

 Data structure c++11

 Simple Matrix Library

 Header file

 Written by Taylor Shin

 Mar. 9th 2017

*****************************************************/

#ifndef CPP11_SIMPLE_MATRIX_HPP
#define CPP11_SIMPLE_MATRIX_HPP

#include <memory>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>

#include "utils.hpp"
#include "btree.hpp"

/* Index key */
struct IndKey {
	ULLONG x;
	ULLONG y;

	bool operator== (const IndKey& o) const
	{
		if (x == o.x && y == o.y) return true;
		else return false;
	}

	bool operator!= (const IndKey& o) const
	{
		return !(*this == o);
	}

	bool operator< (const IndKey& o) const
	{
		if ( x < o.x ) return true;
		else if ( x == o.x ) {
			if ( y < o.y ) return true;
			else return false;
		}
		else return false;
	}

	bool operator<= (const IndKey& o) const
	{
		return (*this < o || *this == o);
	}

	bool operator> (const IndKey& o) const
	{
		if ( x > o.x ) return true;
		else if ( x == o.x ) {
			if ( y > o.y ) return true;
			else return false;
		}
		else return false;
	}

	bool operator>= (const IndKey& o) const
	{
		return (*this > o || *this == o);
	}

	ULLONG operator() (int i)
	{
		if (i == 0) return x;
		else if (i == 1) return y;
		else throw std::invalid_argument("KeyInd: Index out of range!!");
	}

	IndKey& operator= (const IndKey& o)
	{
		x = o.x; y = o.y;
		return *this;
	}

	std::string Print() {
		std::stringstream ss;
		ss << "X: " << x << ", Y: " << y;
		return ss.str();
	}

	IndKey()
	{ x = 0; y = 0; }

	IndKey(const ULLONG& a, const ULLONG& b)
	{ x = a; y = b; }
};


/* Simple Matrix class (Uses binary tree... yeah, sparse matrix) */
template <class T>
class Matrix {
public:
	std::unique_ptr<BTree<T, IndKey>> BTData;

	ULLONG rows;
	ULLONG cols;

	/* Access */
	T& At(const ULLONG& ir, const ULLONG& ic);
	int Set(const ULLONG& ir, const ULLONG& ic, T& d); /* Set data */
	int Set(const ULLONG& ir, const ULLONG& ic,
		std::shared_ptr<T> pd); /* Set data with pointer */

	ULLONG& Rows() const { return rows; }
	ULLONG& Cols() const { return cols; }
	IndKey& Size() const { return IndKey(rows, cols); }
	std::unique_ptr<BTree<T, IndKey>> GetTree() const { return BTData; }
	bool Chk(const ULLONG& ir, const ULLONG& ic) const;

	/* Operator overloading */
	T& operator () (const ULLONG& ir, const ULLONG& ic); /* Same as At */
	Matrix<T>& operator= (const Matrix<T>& m); /* Assignment operator */
	Matrix<T>& operator= (Matrix<T>&& m) noexcept;

	/* Constructors and destructors */
	Matrix();
	Matrix(const ULLONG size_r, const ULLONG size_c, const T& init_data);
	Matrix(const Matrix<T>& m);
	Matrix(Matrix<T>&& m) noexcept;
	virtual ~Matrix();
};





#include "matrix.tcc"

#endif /* Include guard */
