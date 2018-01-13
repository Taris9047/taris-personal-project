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
#include <vector>

#include <cmath>
#include <ctime>

#include "src/utils.hpp"

#include "src/btree.hpp"
#include "src/list.hpp"
#include "src/region.hpp"

#define DEFALUT_ROWS 200
#define DEFAULT_COLS 250

/* Some dummy data class */
class Dummy
{
public:
	int x;
	int y;
	double efield;
	double potential;
	int key;

	int keygen() { return (x+y)*(x+y+1)/2 + x; }

	std::string Print() {
		std::stringstream ss;
		ss << "Key: " << key << ",\t" \
			<< "X: " << x << ",\t" \
			<< "Y: " << y << ",\t" \
			<< "E-Field: " << efield << ",\t" \
			<< "Potential: " << potential;
		return ss.str();
	}

	int GetKey() { return key; }

	Dummy(int x, int y, double ef, double pot) :
		x(x), y(y), efield(ef), potential(pot)
	{
		key = keygen();
	}
	Dummy() : x(0), y(0), efield(0.0), potential(0.0)
	{
		x = (int)std::rand()%DEFALUT_ROWS;
		y = (int)std::rand()%DEFAULT_COLS;
		efield = std::sqrt(std::pow((double)std::rand()/DEFALUT_ROWS, 2.0));
		potential = std::sqrt(std::pow((double)std::rand()/DEFAULT_COLS, 2.0));
		key = keygen();
	}
	Dummy(int A)
	{
		x = A;
		y = 0;
		efield = std::sqrt(std::pow((double)std::rand()/DEFALUT_ROWS, 2.0));
		potential = std::sqrt(std::pow((double)std::rand()/DEFAULT_COLS, 2.0));
		key = keygen();
	}
	Dummy(int A, int B)
	{
		x = A;
		y = B;
		efield = std::sqrt(std::pow((double)std::rand()/DEFALUT_ROWS, 2.0));
		potential = std::sqrt(std::pow((double)std::rand()/DEFAULT_COLS, 2.0));
		key = keygen();
	}
	virtual ~Dummy() {;}
};

/* The test class */
class DataStrTest
{
private:
	std::vector<Dummy> ldata;
	std::vector<std::vector<Dummy>> data;

	ULLONG rows;
	ULLONG cols;

	std::unique_ptr<List<Dummy>> list;
	std::unique_ptr<BTree<Dummy, int>> btree;
	std::unique_ptr<Region<Dummy>> region;

	std::vector<int> index_memory;
	int index_mem_size;
	void set_ind_mem(int size)
	{
		int i;

		index_memory.reserve(size);
		index_mem_size = size;

		for (i=0; i<size; ++i) this->index_memory[i] = -1;
	}
	void del_ind_mem()
	{
		index_memory.empty();
		this->index_mem_size = 0;
	}
	int rand_ind(int max_index)
	{
		int tmp;
		for (;;) {
			tmp = (int)std::rand()%max_index;
			if (!ind_dup(tmp)) break;
		}
		return tmp;
	}
	bool ind_dup(int num)
	{
		int i;
		for (i=0; i<this->index_mem_size; ++i) {
			if (num == this->index_memory[i]) return true;
		}
		return false;
	}

public:
	void TestList();
	void TestBTree();
	void TestMesh();

	DataStrTest();
	DataStrTest(ULLONG rows, ULLONG cols);
	virtual ~DataStrTest();
};

#endif /* Include guard */
