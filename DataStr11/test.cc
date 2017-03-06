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

#include "test.h"

#define DEFALUT_ROWS 200
#define DEFAULT_COLS 250

/**********************************************
 DataStrTest Implementations
 **********************************************/

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
	for (i=0; i<rows; ++i) {
		this->list->Push(ldata[i]);
	}
	std::cout << "[List] Pushed " << rows << " data." << std::endl;

	/* Pop test */
	std::cout << "[List] Testing Pop" << std::endl;
	for (i=0; i<rows/2; ++i) {
		this->list->Pop();
	}
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


/* Constructors and Destructors */
DataStrTest::DataStrTest() : \
	ldata(nullptr),
	data(nullptr),
	list(nullptr),
	rows(0),
	cols(0)
{}

DataStrTest::DataStrTest(ULLONG rows, ULLONG cols) : \
	DataStrTest()
{
	this->rows = rows;
	this->cols = cols;

	ULLONG i, j;

	ldata = new Dummy[this->rows];
	for (i=0; i<this->rows; ++i) ldata[i] = Dummy();

	data = new Dummy*[this->rows];
	for (i=0; i<this->rows; ++i) {
		data[i] = new Dummy[this->cols];
		for (j=0; j<this->cols; ++j) {
			data[i][j] = Dummy();
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
    std::cout << "*** Data structure for myTCAD test program ***" << std::endl;

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

	std::cout << "[Test] Initializing " \
		<< rows << " x " << cols \
		<< " of random dummy data..." << std::endl;
	std::unique_ptr<DataStrTest> DST = \
		std::make_unique<DataStrTest>(rows, cols);

	/* Testing List */
	DST->TestList();

    return 0;
}
