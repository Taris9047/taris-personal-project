/***********************************************

 Reducer

 Header file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#ifndef MAPREDUCE_REDUCER_H
#define MAPREDUCE_REDUCER_H

#include "utils.h"
#include "dparser.h"
#include "pth_handle.h"
#include "list.h"
#include "btree.h"

/* Pixel data struct */
typedef struct _pixel_data {
  ULONG x;
  ULONG y;
  ULONG gs;
} pixel_data;
typedef pixel_data* PixelData;
/* Constructors and Destructors */
PixelData NewPixelData(const ULONG x, const ULONG y, const ULONG gs);
int DeletePixelData(PixelData pd);

/* Dummy image data container */
typedef struct _img_data {
  ULONG x_size;
  ULONG y_size;
  char** label;
  mapped_key_t* ts;

  PixelData* pix_data;

  BTree pixel_data;
  unsigned long long n_entries;

} img_data;
typedef img_data* ImgData;
/* Constructors and Destructors */
ImgData NewImgData(List k);
int DeleteImgData(ImgData img_data);


/* Reducer args struct */
typedef struct _reducer_args {
  List keys;
  ImgData image_data;
} reducer_args;
typedef reducer_args* RDArgs;
/* Constructors and Destructors */
RDArgs NewRDArgs(List k);
int DeleteRDArgs(RDArgs rda);

/*
  Reducer
  generates an image data set with given
  list of keys
  --> another pthread worker
*/
void* reducer(void* args);
// Single thread version --> works without pthread
void* reducer_single(RDArgs args);

/* Keygen for reducer */
ULLONG pixel_keygen(const ULONG a, const ULONG b);

/* Img file writer */
int ImgDataWriter(ImgData img_data, char* base_name);






#endif /* Include guard */
