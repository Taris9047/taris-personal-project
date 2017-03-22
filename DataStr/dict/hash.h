/***************************************

  Dictionary data structure for C
  -> Hashing algorithm

  Header file

  Taylor Shin, Mar. 22th 2017

 ***************************************/
#ifndef C_IMPLEMENTATION_HASHING_H
#define C_IMPLEMENTATION_HASHING_H

/* Mappers for various data types */
#define Mapper(T) \
  _Generic ( (T), \
  int:mapper_i, \
  unsigned int:mapper_ui, \
  long:mapper_l, \
  unsigned long:mapper_ul, \
  unsigned long long:mapper_ull, \
  char*:mapper_str, \
  void*:mapper, \
  default:mapper \
) (T)









#endif /* Include guard */
