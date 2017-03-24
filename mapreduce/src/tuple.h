/***************************************

  A Simple Tuple Datastructure

  Header file

  Taylor Shin, Mar. 242 2017

***************************************/
#ifndef C_IMPLEMENTATION_TUPLE_H
#define C_IMPLEMENTATION_TUPLE_H

/* datatype def */
typedef void* tuple_data_t;

/* Tuple struct */
typedef struct _tuple_struct {
  tuple_data_t* data;
  unsigned int len; /* typically 2, but who knows? */
} tuple_struct;
typedef tuple_struct* Tuple;

/* Constructors and Destructors */
Tuple NewTuple(const unsigned int data_len);
Tuple NewTupleData(const unsigned int data_len, const void* data_set[]);
int DeleteTuple(Tuple t);

/* Methods */
tuple_data_t TAt(Tuple t, int ind);
int TSet(Tuple t, int ind, const tuple_data_t data);

#endif /* Include guard */
