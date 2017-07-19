/********************************************

  Numeric format

  Implementation file

  Taylor Shin

  July 13th 2017

*********************************************/

#include "num.hpp"

/* Read Data */
void* Num::Data() const
{
  void* p_rtn;
  switch (tp) {
    case VOID_T:
      p_rtn = data.vptr;
      break;
    case INT_T:
      p_rtn = data.iptr;
      break;
    case LONG_T:
      p_rtn = data.lptr;
      break;
    case FLT_T:
      p_rtn = data.fptr;
      break;
    case DBL_T:
      p_rtn = data.dptr;
      break;
    default:
      p_rtn = nullptr;
      break;
  }

  return p_rtn;
}

/* Get size */
size_t Num::Size() const
{
  switch (tp) {
    case VOID_T:
      return sizeof(void*);
    case INT_T:
      return sizeof(int);
    case LONG_T:
      return sizeof(uint64_t);
    case FLT_T:
      return sizeof(float);
    case DBL_T:
      return sizeof(double);
    default:
      return 0;
  }
}

/* Get type */
ptype Num::Type() const
{
  return tp;
}

/* Set data */
void Num::Set(const int& new_data)
{
  data.iptr = (int*)malloc(sizeof(int));
  *data.iptr = new_data;
  tp = INT_T;
}


/* Constructors and Destructors */
Num::Num() : data({}), tp(VOID_T) {;}
Num::Num(const int& given_data) : Num() { Set(given_data); }


Num::Num(const Num& other) : Num()
{
  void* p_data = other.Data();
  tp = other.Type();
  switch (tp) {
    case VOID_T: data.vptr = p_data; break;
    case INT_T: data.iptr = (int*)p_data; break;
    case LONG_T: data.lptr = (int64_t*)p_data; break;
    case FLT_T: data.fptr = (float*)p_data; break;
    case DBL_T: data.dptr = (double*)p_data; break;
    default: data.vptr = nullptr; break;
  }
}

Num::Num(Num&& other) noexcept
{
  void* p_data = other.Data();
  tp = other.Type();
  switch (tp) {
    case VOID_T: data.vptr = p_data; break;
    case INT_T: data.iptr = (int*)p_data; break;
    case LONG_T: data.lptr = (int64_t*)p_data; break;
    case FLT_T: data.fptr = (float*)p_data; break;
    case DBL_T: data.dptr = (double*)p_data; break;
    default: data.vptr = nullptr; break;
  }
}

Num& Num::operator= (const Num& other)
{
  Num tmp(other);
  *this = std::move(tmp);
  return *this;
}

Num::~Num() noexcept
{
  switch (tp) {
    case VOID_T:
      free(data.vptr);
      break;
    case INT_T:
      free(data.iptr);
      break;
    case LONG_T:
      free(data.lptr);
      break;
    case FLT_T:
      free(data.fptr);
      break;
    case DBL_T:
      free(data.dptr);
      break;
    default:
      break;
  }
}
