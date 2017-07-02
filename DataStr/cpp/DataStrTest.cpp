/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Implementation file.

	Written by Taylor Shin
	June 22th 2017

******************************************/

#include "DataStrTest.hpp"

/* The main function */
int main (int argc, char* argv[])
{
  /* Argument check up */
  uint64_t data_length = DUMMY_DATA_LENGTH;
  if (argc > 1) {
    std::stringstream(argv[1]) >> data_length;
  }

  std::cout << "Data length: " << data_length << std::endl;

  /* Preparing a crappy dummy data */
  std::cout << "Preparing some dummy data!" << std::endl;
  std::vector<double*> dummy_vec(data_length);
  uint64_t i;
  double* dummy_array = new double[data_length];
  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i<data_length; ++i) {
      dummy_array[i] = (double)std::rand();
    }
  } /* #pragma omp parallel */

  List* pList = new List();

  std::cout << "List length: " << pList->Len() << std::endl;

  /* Assigning dummy data to list */
  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i<data_length; ++i)
      pList->Push(&dummy_array[i]);
  } /* #pragma omp parallel */

  std::cout << "List length after populting: " \
    << pList->Len() << std::endl;

  delete pList;

  delete [] dummy_array;

  return 0;
}
