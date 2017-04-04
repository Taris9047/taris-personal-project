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
PixelData NewPixelData(const ULONG x, const ULONG y, const ULONG gs)
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

  PObj tmp_po = NULL;
  //Key tmp_k = NULL;
  PixelData max_pix_data = NULL;
  ULLONG i;

  ImgData img_data = (ImgData)malloc(sizeof(img_data));
  assert(img_data);

  img_data->n_entries = LLen(key_list);
  Key* key_ary = (Key*)LtoA(key_list);

  img_data->pixel_data = NewBTree();

  img_data->label = \
    (char**)malloc(sizeof(char*)*img_data->n_entries);
  assert(img_data->label);
  for (i=0; i<img_data->n_entries; ++i) {
    //tmp_k = (Key)LAt(key_list, i);
    //tmp_po = tmp_k->point_data;
    tmp_po = key_ary[i]->point_data;
    img_data->label[i] = \
      (char*)malloc(sizeof(char)*(strlen(tmp_po->label)+1));
    assert(img_data->label[i]);
    //img_data->label[i] = strdup(tmp_po->label);
    strcpy(img_data->label[i], tmp_po->label);
  }

  img_data->ts = \
    (mapped_key_t*)malloc(
      sizeof(mapped_key_t)*img_data->n_entries);
  assert(img_data->ts);
  for (i=0; i<img_data->n_entries; ++i) {
    // tmp_k = (Key)LAt(key_list, i);
    // tmp_po = tmp_k->point_data;
    tmp_po = key_ary[i]->point_data;
    img_data->ts[i] = tmp_po->ts;
  }

  img_data->pix_data = \
    (PixelData*)malloc(
      sizeof(PixelData)*img_data->n_entries);
  assert(img_data->pix_data);
  for (i=0; i<img_data->n_entries; ++i) {
    // tmp_k = (Key)LAt(key_list, i);
    // tmp_po = tmp_k->point_data;
    tmp_po = key_ary[i]->point_data;
    img_data->pix_data[i] = \
      NewPixelData(tmp_po->x, tmp_po->y, tmp_po->gs);
    BTInsert(
      img_data->pixel_data,
      img_data->pix_data[i],
      pixel_keygen(tmp_po->x, tmp_po->y) );
  }

  max_pix_data = (PixelData)BTGetMax(img_data->pixel_data);
  img_data->x_size = max_pix_data->x+1;
  img_data->y_size = max_pix_data->y+1;

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
void* reducer(void* args)
{
  pth_args _args = (pth_args)args;
  RDArgs rd_data_set = (RDArgs)_args->data_set;
  pid_t my_pid = _args->pid;

  ImgData i_data = NewImgData(rd_data_set->keys);
  rd_data_set->image_data = i_data;

  arg_bundle_delete(_args);

  pthread_exit((void*)&my_pid);

  return NULL;
}

// Single thread version --> works without pthread
void* reducer_single(RDArgs args)
{
  ImgData i_data = NewImgData(args->keys);
  args->image_data = i_data;

  return NULL;
}

/***********************************************
 Utilities for reducer
************************************************/
/* Keygen */
mapped_key_t pixel_keygen(const ULONG a, const ULONG b)
{
  return (mapped_key_t)( (a+b+1)*(a+b)/2 + b );
}

/* Img file writer */
int ImgDataWriter(ImgData img_data, char* base_name)
{
  assert(base_name);
  assert(img_data);

  // char* fbase = NULL;
  // char delim = '_';
  // char pdelim = ' ';
  // char fext[5] = ".txt";

  printf("Writing some data...\n");
  // Just dummy... we don't have to waste time for an example code.

  //
  // strcpy(fbase, base_name);
  // strcat(fbase, &delim);
  //
  // char* str_timestamp;
  // int str_ts_len = snprintf(NULL, 0, "%llu", img_data->ts);
  // str_timestamp = (char*)malloc(sizeof(char)*str_ts_len);
  // assert(str_timestamp);
  // sprintf(str_timestamp, "%llu", img_data->ts);
  //
  // strcat(fbase, str_timestamp);
  // strcat(fbase, &delim);
  // strcat(fbase, img_data->label);
  // strcat(fbase, fext);
  // char* fname = fbase;
  //
  // char* buffer;
  //
  // FILE* fp;
  // fp = fopen(fname, "a"); /* Data are read out not so concurrently. So, append */
  // if (!fp) {
  //   fprintf(stderr, "reducer: Error, can't open img txt file!!\n");
  //   exit(-1);
  // }
  //
  // ULONG i, j;
  // char *str_x, *str_y, *str_gs;
  // int str_x_len, str_y_len, str_gs_len;
  // PixelData tmp_pixel = NULL;
  // for (i=0; i<img_data->x_size; ++i) {
  //   for (j=0; j<img_data->y_size; ++j) {
  //     buffer = NULL;
  //     strcpy(buffer, str_timestamp);
  //     strcat(buffer, &pdelim);
  //     strcat(buffer, img_data->label);
  //     strcat(buffer, &pdelim);
  //
  //     tmp_pixel = \
  //       (PixelData)BTSearch(
  //         img_data->pixel_data, pixel_keygen(i, j));
  //
  //     str_x_len = snprintf(NULL, 0, "%lu", tmp_pixel->x);
  //     str_y_len = snprintf(NULL, 0, "%lu", tmp_pixel->y);
  //     str_gs_len = snprintf(NULL, 0, "%lu", tmp_pixel->gs);
  //     str_x = (char*)malloc(sizeof(char)*str_x_len);
  //     str_y = (char*)malloc(sizeof(char)*str_y_len);
  //     str_gs = (char*)malloc(sizeof(char)*str_gs_len);
  //
  //     strcat(buffer, str_x);
  //     strcat(buffer, &pdelim);
  //     strcat(buffer, str_y);
  //     strcat(buffer, &pdelim);
  //     strcat(buffer, str_gs);
  //
  //     fprintf(fp, "%s\n", buffer);
  //
  //     free(str_x);
  //     free(str_y);
  //     free(str_gs);
  //     DeletePixelData(tmp_pixel);
  //   }
  // }
  //
  // free(str_timestamp);
  // fclose(fp);
  return 0;
}
