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
PixelData NewPixelData(ULONG x, ULONG y, ULONG gs);
int DeletePixelData(PixelData pd);

/* Dummy image data container */
typedef struct _img_data {
  ULONG x_size;
  ULONG y_size;
  char* label;
  ULLONG ts;
  BTree pixel_data;
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

/* Keygen for reducer */
inline ULLONG pixel_keygen(ULONG a, ULONG b);

/* Img file writer */
int ImgDataWriter(ImgData img_data, char* base_name);






#endif /* Include guard */
