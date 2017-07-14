/*******************************************

  Utils Header

********************************************/

#ifndef MATRIX_CUDA_UTILS_HEADER
#define MATRIX_CUDA_UTILS_HEADER

#include <iostream>
#include <cstdlib>
#include <cstddef>

#include <memory>
#include <vector>

/* Some custom make_unique for c++11 */
#if __cplusplus <= 201103L
namespace std {
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
#endif /* #if __cplusplus <= 201103L */

#endif /* Include guard */
