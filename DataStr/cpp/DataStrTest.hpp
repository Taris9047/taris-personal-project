/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Header file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_TEST_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_TEST_HEADER

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "list.hpp"
#include "btree.hpp"

#define DUMMY_DATA_LENGTH 65535
#define DUMMY_DATA_T double

/* Prepares dummy data */
std::vector<DUMMY_DATA_T> PrepDummy(uint64_t dummy_data_len);

/* Test list */
void TestList(uint64_t dummy_data_len);

/* Test Binary Tree */
void TestBTree(uint64_t dummy_data_len);

#endif /* Include guard */
