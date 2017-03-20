/***********************************************

 Reducer

 Implementation file

 Written by Taylor Shin

 March. 14th 2017

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include "mapper.h"
#include "reducer.h"

/***********************************************
 Pixel data handler
************************************************/
PixelData NewPixelData(ULONG x, ULONG y, ULONG gs)
{
  PixelData pd = (PixelData)malloc(sizeof(pixel_data));
  assert(pd);

  pd->x = x;
  pd->y = y;
  pd->gs = gs;

  return pd;
}

int DeletePixelData(PixelData pd)
{
  assert(pd);
  free(pd);
  return 0;
}


/***********************************************
 Image data handler
************************************************/
ImgData NewImgData(List key_list)
{
  assert(key_list);

  PixelData pix_data;
  PObj tmp_po;
  char* tmp_label = NULL;

  BTree pixels = NewBTree();

  ImgData img_data = (ImgData)malloc(sizeof(img_data));
  assert(img_data);

  while(1) {
    Key tmp_k = (Key)LPop(key_list);
    tmp_po = tmp_k->point_data;

    img_data->ts = tmp_po->ts;
    strcpy(tmp_label, tmp_po->label);
    img_data->label = tmp_label;
    pix_data = NewPixelData(tmp_po->x, tmp_po->y, tmp_po->gs);
    BTInsert(pixels, pix_data, pixel_keygen(tmp_po->x, tmp_po->y) );

    DeletePObj(tmp_po);
    if (LLen(key_list) == 0) break;
  }

  pix_data = (PixelData)BTGetMax(pixels);
  img_data->x_size = pix_data->x+1;
  img_data->y_size = pix_data->y+1;

  img_data->pixel_data = pixels;

  return img_data;
}

int DeleteImgData(ImgData img_data)
{
  assert(img_data);

  ULONG i, j;

  free(img_data->label);

  PixelData tmp;
  for (i=0; i<img_data->x_size; ++i) {
    for (j=0; j<img_data->y_size; ++i) {
      tmp = (PixelData)BTSearch(img_data->pixel_data, pixel_keygen(i, j));
      DeletePixelData(tmp);
    }
  }

  DeleteBTree(img_data->pixel_data);

  return 0;
}




/***********************************************
 Reducer Argument struct handler
************************************************/
RDArgs NewRDArgs(List k)
{
  RDArgs rda = (RDArgs)malloc(sizeof(reducer_args));
  assert(rda);
  rda->keys = k;
  return rda;
}
int DeleteRDArgs(RDArgs rda)
{
  assert(rda);
  free(rda);
  return 0;
}

/***********************************************
 Reducer itself - another pthread worker
************************************************/
void reducer(void* args)
{
  pth_args _args = (pth_args)args;
  RDArgs rd_data_set = (RDArgs)_args->data_set;
  pid_t my_pid = _args->pid;

  ImgData i_data = NewImgData(rd_data_set->keys);
  rd_data_set->image_data = i_data;

  arg_bundle_delete(_args);

  pthread_exit((void*)&my_pid);
}

/***********************************************
 Utilities for reducer
************************************************/
/* Keygen */
inline ULLONG pixel_keygen(ULONG a, ULONG b)
{
  return (ULLONG)( (a+b+1)*(a+b)/2 + b );
}

/* Img file writer */
int ImgDataWriter(ImgData img_data, char* base_name)
{
  assert(base_name);
  assert(img_data);

  char* fbase = NULL;
  char delim = '_';
  char pdelim = ' ';
  char fext[5] = ".txt";

  strcpy(fbase, base_name);
  strcat(fbase, &delim);

  char* str_timestamp;
  int str_ts_len = snprintf(NULL, 0, "%llu", img_data->ts);
  str_timestamp = (char*)malloc(sizeof(char)*str_ts_len); assert(str_timestamp);
  sprintf(str_timestamp, "%llu", img_data->ts);

  strcat(fbase, str_timestamp);
  strcat(fbase, &delim);
  strcat(fbase, img_data->label);
  strcat(fbase, fext);
  char* fname = fbase;

  char* buffer;

  FILE* fp;
  fp = fopen(fname, "a"); /* Data are read out not so concurrently. So, append */
  if (!fp) {
    fprintf(stderr, "reducer: Error, can't open img txt file!!\n");
    exit(-1);
  }

  ULONG i, j;
  char *str_x, *str_y, *str_gs;
  int str_x_len, str_y_len, str_gs_len;
  PixelData tmp_pixel = NULL;
  for (i=0; i<img_data->x_size; ++i) {
    for (j=0; j<img_data->y_size; ++j) {
      buffer = NULL;
      strcpy(buffer, str_timestamp);
      strcat(buffer, &pdelim);
      strcat(buffer, img_data->label);
      strcat(buffer, &pdelim);

      tmp_pixel = \
        (PixelData)BTSearch(
          img_data->pixel_data, pixel_keygen(i, j));

      str_x_len = snprintf(NULL, 0, "%lu", tmp_pixel->x);
      str_y_len = snprintf(NULL, 0, "%lu", tmp_pixel->y);
      str_gs_len = snprintf(NULL, 0, "%lu", tmp_pixel->gs);
      str_x = (char*)malloc(sizeof(char)*str_x_len);
      str_y = (char*)malloc(sizeof(char)*str_y_len);
      str_gs = (char*)malloc(sizeof(char)*str_gs_len);

      strcat(buffer, str_x);
      strcat(buffer, &pdelim);
      strcat(buffer, str_y);
      strcat(buffer, &pdelim);
      strcat(buffer, str_gs);

      fprintf(fp, "%s\n", buffer);

      free(str_x);
      free(str_y);
      free(str_gs);
      DeletePixelData(tmp_pixel);
    }
  }

  free(str_timestamp);
  fclose(fp);
  return 0;
}
