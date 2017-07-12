/*****************************************

  Some utility craps.

	Header file.

	Written by Taylor Shin
	July 12th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_UTILS_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_UTILS_HEADER

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
