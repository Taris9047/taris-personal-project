/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-06T08:36:15-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-06T08:43:33-06:00
*/

/****************************************************

 Data structure c++11 implementation test program

 header file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

#ifndef CPP11_TEST_H
#define CPP11_TEST_H

#include <memory>
#include <cassert>
#include <string>
#include <sstream>

#include <cmath>
#include <ctime>

#include "utils.h"

#include "btree.h"
#include "list.h"
#include "mesh.h"

/* Some dummy data class */
class Dummy {
private:
	int x;
	int y;
	double efield;
	double potential;
	int seed;

public:
	std::string Print() {
		std::stringstream ss;
		ss << "X: " << x << ", " \
			<< "Y: " << y << ", " \
			<< "E-Field: " << efield << ", " \
			<< "Potential: " << potential;
		return ss.str();
	}

	Dummy(int x, int y, double ef, double pot) :
		x(x), y(y), efield(ef), potential(pot) {;}
	Dummy() : x(0), y(0), efield(0.0), potential(0.0)
	{
		x = (int)std::rand();
		y = (int)std::rand();
		efield = std::sqrt(std::pow((double)std::rand(), 2.0));
		potential = std::sqrt(std::pow((double)std::rand(), 2.0));
	}
	virtual ~Dummy() {;}
};

/* The test class */
class DataStrTest {
private:
	Dummy* ldata;
	Dummy** data;
	ULLONG rows;
	ULLONG cols;

	std::unique_ptr<List<Dummy>> list;
	// std::unique_ptr<BTree<Dummy>> btree;
	// std::unique_ptr<Mesh<Dummy>> mesh;

public:
	void TestList();
	void TestBTree();
	void TestMesh();

	DataStrTest();
	DataStrTest(ULLONG rows, ULLONG cols);
	~DataStrTest();
};

#endif /* Include guard */
