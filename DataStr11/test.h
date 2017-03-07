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

public:
	int keygen() { return (x+y)*(x+y+1)/2 + x; }

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
	std::unique_ptr<BTree<Dummy, int>> btree;
	// std::unique_ptr<Mesh<Dummy>> mesh;

	int* index_memory;
	int index_mem_size;
	void set_ind_mem(int size) {
		if (index_memory) delete [] index_memory;
		index_memory = new int[size];
		index_mem_size = size;
	}
	void del_ind_mem() {
		if (index_memory) delete [] index_memory;
		index_mem_size = 0;
	}
	int rand_ind(int max_index) {
		int tmp;
		for (;;) {
			tmp = (int)std::rand()%max_index;
			if (!ind_dup(tmp)) break;
		}
		return tmp;
	}
	bool ind_dup(int num) {
		assert(!index_memory || index_mem_size == 0);
		int i;
		for (i=0; i<index_mem_size; ++i) {
			if (num == index_memory[i]) return true;
		}
		return false;
	}

public:
	void TestList();
	void TestBTree();
	void TestMesh();

	DataStrTest();
	DataStrTest(ULLONG rows, ULLONG cols);
	~DataStrTest();
};

#endif /* Include guard */
