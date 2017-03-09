/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:16:26-06:00
*/



/****************************************************

 Data structure c++11 implementation test program

 Implementation file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

#include <cassert>
#include <memory>
#include <string>
#include <iostream>

#include "test.hpp"

/****************************************************
 DataStrTest::Test Methods
*****************************************************/
/* Test functions */
// List test
void DataStrTest::TestList()
{
	/* We'll just use ldata */
	if (!ldata) return;

	ULLONG i, ti;

	/* Destroy previous List */
	if (this->list) this->list = nullptr;

	/* Make a new List */
	this->list = std::make_unique<List<Dummy>>();

	/* Push test */
	std::cout << "[List] Testing Push" << std::endl;
	for (i=0; i<rows; ++i) this->list->Push(ldata[i]);
	std::cout << "[List] Pushed " << rows << " data." << std::endl;

	/* Pop test */
	std::cout << "[List] Testing Pop" << std::endl;
	for (i=0; i<rows/2; ++i) this->list->Pop();
	std::cout << "[List] Popped " << rows/2 << " data." << std::endl;

	/* Access test */
	ULLONG rnd_spots = rows/2/20;
	if (!rnd_spots) rnd_spots = 1;
	ULLONG* rnd_spots_backup = new ULLONG[rnd_spots];
	std::cout << "[List] Accessing data at " \
		<< rnd_spots << " random spots." << std::endl;
	for (i=0; i<rnd_spots; ++i) {
		ti = ((ULLONG)std::rand())%(rows/2);
		std::cout << "[List] Data at " << ti \
			<< ": " << this->list->At(ti).Print() << std::endl;
		rnd_spots_backup[i] = ti;
	}

	/* Reverse test */
	std::cout << "[List] Reverse Test" << std::endl;
	this->list->Reverse();
	for (i=0; i<rnd_spots; ++i) {
		std::cout << "[List] Data at " << rnd_spots_backup[i] \
			<< ": " << this->list->At(rnd_spots_backup[i]).Print() \
			<< std::endl;
	}

	delete [] rnd_spots_backup;
}

// Binary Tree test
void DataStrTest::TestBTree()
{
	/* We'll just use ldata */
	if (!ldata) return;

	ULLONG i, it;
	int key;
	int tmp;

	/* Make a Binary tree */
	this->btree = std::make_unique<BTree<Dummy, int>>();

	std::cout << "[BTree] Testing insert " << std::endl;
	for (i=0; i<rows; ++i) {
		key = ldata[i].GetKey();
		this->btree->Insert(ldata[i], key);
	}
	std::cout << "[BTree] Inserted " << rows << " Dummy data. " << std::endl;

	std::cout << "[BTree] Removal test ... " << std::endl;
	ULLONG sel_items = rows/10;
	std::cout << "[BTree] Selecting " << sel_items << " items..." << std::endl;
	set_ind_mem(sel_items);
	for (i=0; i<sel_items; ++i) {
		tmp = rand_ind(sel_items);
		index_memory[i] = tmp;
		std::cout << "[BTree] Selected " << ldata[tmp].Print() << std::endl;
	}
	for (i=0; i<index_mem_size; ++i) {
		key = ldata[index_memory[i]].GetKey();
		this->btree->Remove(key);
		std::cout << "[BTree] Removed " << ldata[index_memory[i]].Print() << std::endl;
	}

	std::cout << "[BTree] Testing search ... " << std::endl;
	for (i=0; i<index_mem_size; ++i) {
		it = (ULLONG)std::rand()%rows;
		key = ldata[it].GetKey();
		if ( this->btree->pGet(key) != nullptr ) {
			std::cout << "[BTree] Found: " << ldata[it].Print() << std::endl;
		}
		else {
			std::cout << "[BTree] Not Found: " << ldata[it].Print() << std::endl;
		}
	}

	del_ind_mem();
}


// Mesh test
void DataStrTest::TestMesh()
{
	ULLONG i, j, it, jt;

	assert(data);

	std::cout << "[Mesh] Testing mesh..." << std::endl;

}

/****************************************************
 DataStrTest::Constructors and Destructors
*****************************************************/
DataStrTest::DataStrTest() : \
	ldata(nullptr),
	data(nullptr),
	list(nullptr),
	rows(0),
	cols(0),
	index_memory(nullptr),
	index_mem_size(0)
{}

DataStrTest::DataStrTest(ULLONG rows, ULLONG cols) : \
	DataStrTest()
{
	this->rows = rows;
	this->cols = cols;

	ULLONG i, j;

	ldata = new Dummy[this->rows];
	for (i=0; i<this->rows; ++i) ldata[i] = Dummy((int)i);

	data = new Dummy*[this->rows];
	for (i=0; i<this->rows; ++i) {
		data[i] = new Dummy[this->cols];
		for (j=0; j<this->cols; ++j) {
			data[i][j] = Dummy((int)i, (int)j);
		}
	}
}

DataStrTest::~DataStrTest()
{
	ULLONG i;
	if (ldata) delete[] ldata;
	if (data) {
		for (i=0; i<this->rows; ++i) delete [] data[i];
	}
	delete [] data;
}


/**********************************************
 The main function
 **********************************************/
int main(int argc, char* argv[])
{
	ULLONG rows, cols;
	std::string rowstr, colstr;

	// Init random seed
	int t = std::time(NULL);
	std::srand(t);

	if (argc < 3) {
		rows = DEFALUT_ROWS;
		cols = DEFAULT_COLS;
	}
	else if (argc == 3) {
		rowstr = std::string(argv[1]);
		colstr = std::string(argv[2]);
		rows = std::stoi(rowstr);
		cols = std::stoi(colstr);
	}

	std::cout << "*** Data structure for myTCAD test program ***" << std::endl;
	std::cout << std::endl;

	std::cout << "[Test] Initializing " \
		<< rows << " x " << cols \
		<< " of random dummy data..." \
		<< std::endl << std::endl;
	std::unique_ptr<DataStrTest> DST = \
		std::make_unique<DataStrTest>(rows, cols);

	/* Testing List */
	DST->TestList(); std::cout << std::endl;

	/* Testing BTree */
	DST->TestBTree(); std::cout << std::endl;

	/* Testing Mesh */
	DST->TestMesh(); std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "[Test] Test finished." << std::endl;

    return 0;
}
