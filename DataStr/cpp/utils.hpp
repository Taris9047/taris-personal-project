/*****************************************

  Some utility craps.

	Header file.

	Written by Taylor Shin
	July 12th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_UTILS_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_UTILS_HEADER

#include <algorithm>
#include <vector>

/* Find index from a vector */
template <typename T>
size_t find_vec_pos(std::vector<T> vec, T elem)
{
  ptrdiff_t pos = \
    std::distance(
      vec.begin(),
      std::find(vec.begin(), vec.end(), elem));

  return (size_t)pos;
}

/* Delete an element from a vector */
template <typename T>
void erase_vec_elem(std::vector<T> vec, T elem)
{
  vec.erase(
    std::remove(vec.begin(),
    vec.end(), elem), vec.end()
  );
}

/* Make unique */
#include <memory>

#ifndef _MSC_VER

#  if __cplusplus >= 201402L
  using std::make_unique;
#  else
namespace std {
  template <typename T, typename... Ts>
  std::unique_ptr<T> make_unique(Ts&&... params)
  {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
  }
}
#  endif /* __cplusplus */
#endif /* _MSC_VER */


#endif /* Include guard */
