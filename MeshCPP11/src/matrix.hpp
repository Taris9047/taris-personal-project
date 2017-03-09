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
private:
	std::unique_ptr<BTree<T, IndKey>> BTData;

	ULLONG rows;
	ULLONG cols;

public:
	/* Access */
	T& At(const ULLONG ir, const ULLONG ic);
	T& operator () (const ULLONG ir, const ULLONG ic); /* Same as At */
	int Set(const ULLONG ir, const ULLONG ic, T& d); /* Set data */
	Matrix<T>& operator= (const Matrix<T>& m); /* Assignment operator */

	ULLONG Rows() { return rows; }
	ULLONG Cols() { return cols; }
	IndKey Size() { return IndKey(rows, cols); }
	std::unique_ptr<BTree<T, IndKey>> GetTree()
	{ return BTData; }

	/* Constructors and destructors */
	Matrix();
	Matrix(const ULLONG size_r, const ULLONG size_c, const T& init_data);
	Matrix(const Matrix<T>& m);
	virtual ~Matrix();
};





#include "matrix.tcc"

#endif /* Include guard */
