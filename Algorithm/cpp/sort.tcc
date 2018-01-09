/*********************************

    Sort implementations
    
**********************************/

/**** Quick Sort ****/
template<typename T>
void qsort(std::vector<T>& ary, int64_t lo, int64_t hi)
{
    if (lo < hi) {
        auto p = partition<T>(ary, lo, hi);
        qsort<T>(ary, lo, p-1);
        qsort<T>(ary, p+1, hi);
    }
}

template<typename T>
int64_t partition(std::vector<T>& ary, int64_t lo, int64_t hi)
{
    auto pivot = ary[hi];
    auto i = lo-1;
    
    for (auto j=lo; j<hi-1; ++j) {
        if (ary[j] <= pivot) {
            i++;
            std::swap<T>(ary[i], ary[j]);
        }
    }
    
    if (ary[hi] < ary[i+1]) std::swap<T>(ary[i+1], ary[hi]);
    
    return i+1;
}


/**** Merge Sort ****/
template<typename T>
void msort_td(std::vector<T>& ary, int64_t lo, int64_t hi)
{
    if (lo < hi)
    {
        auto mid = (lo+hi)/2;

        msort_td<T>(ary, lo, mid);
        msort_td<T>(ary, mid+1, hi);

        merge_td<T>(ary, lo, mid, hi);
    }
}

template<typename T>
void merge_td(std::vector<T>& ary, int64_t lo, int64_t mid, int64_t hi)
{
    std::vector<T> aryA(mid-lo+1);
    std::vector<T> aryB(hi-mid);
    
    for (auto i=0; i<mid-lo+1; ++i) aryA[i] = ary[lo+i];
    for (auto i=0; i<hi-mid; ++i) aryB[i] = ary[mid+1+i];
    
    auto iA = 0;
    auto iB = 0;
    auto iary = lo;
    
    while (iA < aryA.size() && iB < aryB.size()) {
        if (aryA[iA] <= aryB[iB])
            ary[iary++] = aryA[iA++];
        else
            ary[iary++] = aryB[iB++];
    }
    
    while (iA < aryA.size())
        ary[iary++] = aryA[iA++];
    
    while (iB < aryB.size())
        ary[iary++] = aryB[iB++];
    
}
