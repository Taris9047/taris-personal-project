/*************************************************
  A simple UDP random packet generator

  Purpose
  To emulate massive data influx from the
  VIPIC board.

  --> Custom header definition

  Writen by Taylor Shin

  Nov. 30th 2017

**************************************************/
#ifndef CUSTOM_DATA_HEADER_HEADER_FILE
#define CUSTOM_DATA_HEADER_HEADER_FILE

#include "rand_safe.h"
#include "utils.h"

/*************************************************
  Custom header struct
**************************************************/
typedef struct _vipic_data_header {
  uint16_t src_port;
  uint16_t dest_port;
  uint16_t socket_length;
  uint16_t chksum;
  /* Maybe add some thumbnail for pixel info? */
  // uint16_t row;
  // uint16_t col;
  // uint32_t cnt;
} header __attribute__ ((aligned(8)));
typedef header* Header;

/*************************************************
  Generate a random data with a custom header
**************************************************/
unsigned char* rnd_custom_data_w_header(Header hd, uint16_t worker_iter)
{
  /* At least socket should be longer than header. right? */
  assert(hd->socket_length > sizeof(header));

  /* entire bit array size in bytes */
  uint16_t skt_sz = hd->socket_length;
  uint64_t entire_ary_sz = skt_sz*worker_iter;

  /* The actual bit array */
  unsigned char* pix_data = (unsigned char*)tmalloc(entire_ary_sz);
  assert(pix_data);

  int src_port_sz = sizeof(uint16_t);
  int dest_port_sz = sizeof(uint16_t);
  int socket_length_sz = sizeof(uint16_t);
  int chksum_sz = sizeof(uint16_t);
  int header_sz = src_port_sz+dest_port_sz+socket_length_sz+chksum_sz; /* 8 Bytes */

  uint64_t i, j;
  unsigned char *hdr_buf, *skt_buf;
  /*
    Basically, this part can be just 8 bytes. But let's leave it this way
    for clarity...
  */
  for (i=0; i<worker_iter; ++i) {
    hdr_buf = (unsigned char*)tmalloc(header_sz);
    assert(hdr_buf);

    /* populate header bit array */
    hdr_buf[0] = (hd->src_port >> 8) & 0xFF;
    hdr_buf[1] = (hd->src_port) & 0xFF;
    hdr_buf[2] = (hd->dest_port >> 8) & 0xFF;
    hdr_buf[3] = (hd->dest_port) & 0xFF;
    hdr_buf[4] = (hd->socket_length >> 8) & 0xFF;
    hdr_buf[5] = (hd->socket_length) & 0xFF;
    hdr_buf[6] = (hd->chksum >> 8) & 0xFF;
    hdr_buf[7] = (hd->chksum) & 0xFF;

    /* Let's populate skt_buf (skt_buf = hdr_buf+data_buf) */
    skt_buf = (unsigned char*)tmalloc(skt_sz);
    assert(skt_buf);
    for (j=0; j<skt_sz; ++j) {
      if (j<header_sz) skt_buf[j] = hdr_buf[j];
      /* Populating data portion... just random data */
      else skt_buf[j] = rand_byte();
    }

    /* And then populate the main buffer, pix_data */
    for (j=i*worker_iter; j<(i+1)*worker_iter; ++j) {
      pix_data[j] = skt_buf[j-(i*worker_iter)];
    }

    /* Clean up temporary buffers */
    tfree(hdr_buf);
    tfree(skt_buf);
  } /* for (i=0; i<entire_ary_sz; ++i) */

  return pix_data;
}

#endif /* Include guard */
