/***********************************

    Algorithm study
    
    Sort header file.
    
************************************/
#ifndef ALGORITHM_STUDY_SORT_HEADER
#define ALGORITHM_STUDY_SORT_HEADER

#include <vector>
#include <cstddef>
#include <cstdint>

/* Quick Sort */
template<typename T>
void qsort(std::vector<T>& ary, int64_t lo, int64_t hi);
template<typename T>
int64_t partition(std::vector<T>& ary, int64_t lo, int64_t hi);

/* Merge Sort */
template<typename T>
void msort_td(std::vector<T>& ary, int64_t lo, int64_t hi);
template<typename T>
void merge_td(std::vector<T>& ary, int64_t lo, int64_t mid, int64_t hi);

#include "sort.tcc"

#endif /* Include guard */