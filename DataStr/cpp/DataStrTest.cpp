/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Implementation file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#include "DataStrTest.hpp"

/* Prepares dummy data */
std::vector<DUMMY_DATA_T> PrepDummy(uint64_t dummy_data_len)
{
  std::vector<DUMMY_DATA_T> dummy_vec(dummy_data_len);
  uint64_t i;
  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i<dummy_data_len; ++i) {
      dummy_vec[i] = (DUMMY_DATA_T)std::rand();
    }
  } /* #pragma omp parallel */

  return dummy_vec;
}

/* Test list */
void TestList(uint64_t dummy_data_len)
{
  /* Preparing a crappy dummy data */
  std::cout << "Preparing some dummy data for List!" << std::endl;
  std::vector<DUMMY_DATA_T> dummy_vec = PrepDummy(dummy_data_len);

  List* pList = new List();

  std::cout << "List length: " << pList->Len() << std::endl;

  uint64_t i;

  /* Assigning dummy data to list */
  #pragma omp parallel for
  for (i=0; i<dummy_data_len; ++i)
    pList->Push(&dummy_vec[i]);

  std::cout << "List length after populting: " \
    << pList->Len() << std::endl;

  delete pList;
}

/* Test Binary Tree */
void TestBTree(uint64_t dummy_data_len)
{
  /* Preparing a crappy dummy data */
  std::cout << "Preparing some dummy data for Binary Tree!" << std::endl;
  std::vector<DUMMY_DATA_T> dummy_vec = PrepDummy(dummy_data_len);

  BTree* pBTree = new BTree();

  std::cout << "Binary Tree length: " << pBTree->Size() << std::endl;

  uint64_t i;

  #pragma omp parallel for
  for (i=0; i<dummy_data_len; ++i)
    pBTree->Insert(nullptr, dummy_vec[i]);

  std::cout << "Binary Tree length after populating: " \
    << pBTree->Size() << std::endl;

  delete pBTree;
}


/* The main function */
int main (int argc, char* argv[])
{
  /* Argument check up */
  uint64_t data_length = DUMMY_DATA_LENGTH;
  if (argc > 1) std::stringstream(argv[1]) >> data_length;

  /* Check up given data length */
  std::cout << "Given Data length: " << data_length << std::endl;

  /* Now test list */
  TestList(data_length);

  /* Then test binary tree */
  TestBTree(data_length);

  return 0;
}
