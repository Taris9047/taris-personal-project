/**
 * @Author: taris
 * @Date:   2017-05-05T07:14:12-05:00
 * @Last modified by:   taris
 * @Last modified time: 2017-05-05T07:21:18-05:00
 */



/*************************************

  Pseudo DAC - main program

  Implementation file

  Written by:
  Taylor Shin
  May. 3rd 2017

**************************************/

#define _GNU_SOURCE
#include <getopt.h>
#include <locale.h>

#include "psDAC.h"
#include "dat_file_reader.h"

/*************************************
  Help info...
**************************************/
int print_help()
{
  printf("psDAC [options]\n");
  printf("-a\tHost address. Default: %s\n", DEFAULT_HOST);
  printf("-p\tDefines port number. Default is %d. Accepts 10000 to 65535\n", DEFAULT_PORT);
  printf("-f\tInput Datafile. Default is %s. Accepts almost any archive format thanks to libarchive.\n", DEFAULT_DATAFILE);
  printf("-i\tNumber of iterations. Default is %d.\n", DEFAULT_ITERATION);
  printf("-o\tOutput file. Default is %s.\n", DEFAULT_OUTPUT_FILE);
  printf("-d\tDemon mode. Runs the server indefinitely.\n");
  printf("-v\tVerbose the work. Slows down the job a lot. No verbose in default operation.\n");
  printf("-h\tPrints this message.\n");
  printf("\n");
  return 0;
}

char* status_report(psDAC_Options pdo)
{
  assert(pdo);
  char* str;
  asprintf(&str,
    "===================================\n"
    "psDAC Info.\n"
    "Host addr.:\t%s\n"
    "Port #:\t\t%d\n"
    "Data File:\t%s\n"
    "Iteration:\t%lu\n"
    "Threads:\t%lu\n"
    "Outfile Name:\t%s\n"
    "Chunk Mode:\t%s\n"
    "Demon Mode:\t%s\n"
    "===================================\n",
    pdo->host_addr,
    pdo->port_number,
    pdo->data_file,
    pdo->iteration,
    pdo->n_threads,
    pdo->outf_name,
    pdo->chunk_mode?"true":"false",
    pdo->demon_mode?"true":"false"
  );

  return str;
}

/*************************************
  Option list constructor/destructor
**************************************/
psDAC_Options NewpsDAC_Options(int argc, char* argv[])
{
  psDAC_Options pdo = (psDAC_Options)tmalloc(sizeof(psDAC_options));

  pdo->port_number = DEFAULT_PORT;
  pdo->data_file = strdup(DEFAULT_DATAFILE);
  pdo->verbose = false;
  pdo->host_addr = strdup(DEFAULT_HOST);
  pdo->iteration = DEFAULT_ITERATION;
  pdo->outf_name = strdup(DEFAULT_OUTPUT_FILE);
  pdo->n_threads = DEFAULT_THREADS;
  pdo->chunk_mode = false;
  pdo->demon_mode = false;

  int opt;
  while ( (opt = getopt(argc, argv, "p:f:a:i:o:t:dcvh")) != -1 ) {
    switch (opt) {
    case 'p':
      pdo->port_number = (int)atoi(optarg);
      break;
    case 'f':
      tfree(pdo->data_file);
      pdo->data_file = strdup(optarg);
      break;
    case 'a':
      tfree(pdo->host_addr);
      pdo->host_addr = strdup(optarg);
      break;
    case 'i':
      pdo->iteration = (uint64_t)atoi(optarg);
      break;
    case 'o':
      tfree(pdo->outf_name);
      pdo->outf_name = strdup(optarg);
      break;
    case 'v':
      pdo->verbose = true;
      break;
    case 't': /* Dropped multithrading attempt */
      //pdo->n_threads = (uint64_t)atoi(optarg);
      pdo->n_threads = 1;
      break;
    case 'd':
      pdo->demon_mode = true;
      break;
    case 'c':
      pdo->chunk_mode = true;
      break;
    case 'h':
      print_help();
      exit(EXIT_FAILURE);
    default:
      print_help();
      exit(EXIT_FAILURE);
    } /* switch (opt) */
  } /* while ( (opt = getopt(argc, argv, "p:f:i:o:t:cvh")) != -1 ) */

  return pdo;
}

int DeletepsDAC_Options(psDAC_Options pdo)
{
  assert(pdo);
  tfree(pdo->data_file);
  tfree(pdo->outf_name);
  tfree(pdo);
  return 0;
}

/*************************************
  The server routine - Static stuff
**************************************/
// static void t_msg_free(void* data, void* hint)
// {
//   if (data) tfree(data);
// }

/*************************************
  The server routine
**************************************/
int run_psDAC(psDAC_Options pdo)
{
  if (!pdo) {
    fprintf(stderr, "Invalid options!!\n");
    return -1;
  }

  if (!(pdo->port_number >= 10000 && pdo->port_number <= 65535)) {
    fprintf(stderr, "Port number must be within 10000 - 65535 range\n");
    return -1;
  }

  char* server_addr;
  int server_addr_str_len;
  bool verbose = pdo->verbose;
  char* status_str = status_report(pdo);

  fprintf(stdout, "%s\n", status_str);

  server_addr_str_len = snprintf(NULL, 0, "tcp://%s:%d", pdo->host_addr, pdo->port_number);
  server_addr = (char*)tmalloc(sizeof(char)*(server_addr_str_len+1));
  sprintf(server_addr, "tcp://%s:%d", pdo->host_addr, pdo->port_number);

  fprintf(stdout, "Running Pseudo DAC server on %s\n", server_addr);
  fprintf(stdout, "Preparing data from %s\n", pdo->data_file);
  DataContainer dtc = NewDataContainer(pdo->data_file);
  fprintf(stdout, "Data has been prepared!!\n");
  PrintDataContainer(dtc);

  /* Now the server stuff */
  void *context = zmq_ctx_new();
  void *data_publisher = zmq_socket(context, ZMQ_PUSH);
  int rc = zmq_bind(data_publisher, server_addr);
  // zmq_setsockopt(data_publisher, ZMQ_SNDHWM, "", 0);
  if (rc!=0) {
    fprintf(stderr, "zmq_bind failed with code [%d]...\n", rc);
    return rc;
  }

  /* Do run server here */
  unsigned char* volatile segment;
  volatile size_t seg_len;
  uint64_t i, iter;
  zmq_msg_t msg;
  uint64_t delta_us, transfer_rate, delta_us_seg, tot_entry_len = 0;

  // unsigned char** seg_ary = (unsigned char**)LtoA(dtc->entries);
  // size_t** seg_len_ary = (size_t**)LtoA(dtc->entry_len);

  FILE *outf_fp;

  /* prepare output file header */
  if (!pdo->demon_mode) {
    outf_fp = fopen(pdo->outf_name, "w");
    fprintf(outf_fp, "%s\n", status_str);
    fprintf(outf_fp, FILE_HEADER);
    fclose(outf_fp);
  }

  struct timespec start, end;
  for (iter=0; iter<pdo->iteration; ++iter) {
    delta_us = 0;
    for (i=0; i<dtc->entries->len; ++i) {
      segment = (unsigned char*)LAtSeq(dtc->entries, i);
      seg_len = *(uint64_t*)LAtSeq(dtc->entry_len, i);
      if (verbose) {
        fprintf(
          stdout, "Sending... [%lu/%lu] (%.2f %%)",
          i+1, dtc->entries->len,
          (float)(i+1)*100/dtc->entries->len);
      }
      rc = zmq_msg_init_data(&msg, segment, seg_len, NULL, NULL);
      if (rc) ERROR("psDAC: zmq_msg_init_data failed!!", rc);

      clock_gettime(CLOCK_MONOTONIC_RAW, &start);
      rc = zmq_msg_send(&msg, data_publisher, 0);
      clock_gettime(CLOCK_MONOTONIC_RAW, &end);
      if (rc!=seg_len) ERROR("psDAC: zmq_msg_send failed!!", rc);

      delta_us_seg = \
        (end.tv_sec-start.tv_sec)*1000000+(end.tv_nsec-start.tv_nsec)/1000;

      delta_us += delta_us_seg;

      if (verbose) {
        fprintf(stdout, "\r");
        fflush(stdout);
      }

      tot_entry_len += seg_len;

      if (pdo->demon_mode) iter = 0;
    } /* for (i=0; i<dtc->entries->len; ++i) */

    LResetCursor(dtc->entries);
    LResetCursor(dtc->entry_len);

    fprintf(
      stdout,
      "[%lu/%lu] Iteration finished!!\n",
      iter+1, pdo->iteration);
    fprintf(stdout, "\n");

    fprintf(stdout, "Execution time: %'lu us\n", delta_us);
    transfer_rate = (uint64_t)tot_entry_len*8/((double)delta_us/1000000);
    fprintf(stdout, "Transfer Rate: %'lu bps\n", transfer_rate);

    if (!pdo->demon_mode) {
      outf_fp = fopen(pdo->outf_name, "a");
      fprintf(
        outf_fp, "%lu,%zu,%lu,%lu\n", iter+1, seg_len, delta_us, transfer_rate);
      fclose(outf_fp);
    }

  } /* for (iter=0; iter<dtc->entries->len; ++iter) */

  // tfree(seg_ary);
  // tfree(seg_len_ary);

  /* Cleaning up */
  fprintf(stdout, "Closing server...\n");
  zmq_close(data_publisher);
  zmq_ctx_destroy(context);
  tfree(server_addr);
  tfree(status_str);

  fprintf(stdout, "Cleaning up data...\n");
  DeleteDataContainer(dtc);

  return 0;
}

/*************************************
  The server routine - Sending chunk
**************************************/
int run_psDAC_chunk(psDAC_Options pdo)
{
  if (!pdo) {
    fprintf(stderr, "Invalid options!!\n");
    return -1;
  }

  if (!(pdo->port_number >= 10000 && pdo->port_number <= 65535)) {
    fprintf(stderr, "Port number must be within 10000 - 65535 range\n");
    return -1;
  }

  char* server_addr;
  int server_addr_str_len;
  bool verbose = pdo->verbose;
  char* status_str = status_report(pdo);

  fprintf(stdout, "%s\n", status_str);

  server_addr_str_len = snprintf(NULL, 0, "tcp://*:%d", pdo->port_number);
  server_addr = (char*)tmalloc(sizeof(char)*(server_addr_str_len+1));
  sprintf(server_addr, "tcp://*:%d", pdo->port_number);

  fprintf(stdout, "Running Pseudo DAC server on %s\n", server_addr);
  fprintf(stdout, "Preparing data from %s\n", pdo->data_file);
  DataContainer dtc = NewDataContainer(pdo->data_file);
  fprintf(stdout, "Data has been prepared!!\n");
  PrintDataContainer(dtc);

  /* Now the server stuff */
  void *context = zmq_ctx_new();
  void *data_publisher = zmq_socket(context, ZMQ_PUSH);
  int rc = zmq_bind(data_publisher, server_addr);
  // zmq_setsockopt(data_publisher, ZMQ_SNDHWM, "", 0);
  if (rc) ERROR("zmq_bind", rc);

  /* Do run server here */
  unsigned char* data_chunk;
  uint64_t data_chunk_len, iter;
  zmq_msg_t msg;
  uint64_t delta_us, transfer_rate;

  fprintf(stdout, "Making a singzmq_msg_sendle chunk of data!\n");
  RawDataChunk(dtc, &data_chunk, &data_chunk_len);
  fprintf(stdout, "Data size: %lu bytes...\n\n", data_chunk_len);

  FILE *outf_fp;
  /* prepare output file header */
  outf_fp = fopen(pdo->outf_name, "w");
  fprintf(outf_fp, "%s\n", status_str);
  fprintf(outf_fp, FILE_HEADER);
  fclose(outf_fp);

  struct timespec start, end;
  for (iter=0; iter<pdo->iteration; ++iter) {

    if (verbose) {
      fprintf(
        stdout, "Sending... a chunk of %lu Bytes\n", data_chunk_len);
    }

    rc = \
      zmq_msg_init_data(&msg, data_chunk, data_chunk_len, NULL, NULL);
    if (rc) ERROR("psDAC: zmq_msg_init_data failed!!\n", rc);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    //rc = zmq_send(data_publisher, &msg, data_chunk_len, 0);
    rc = zmq_msg_send(&msg, data_publisher, 0);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    if (rc!=data_chunk_len) ERROR("psDAC: zmq_msg_send failed!!", rc);

    fprintf(
      stdout,
      "[%lu/%lu] Iteration finished!!\n",
      iter+1, pdo->iteration);
    fprintf(stdout, "\n");

    delta_us = \
      (end.tv_sec-start.tv_sec)*1000000+(end.tv_nsec-start.tv_nsec)/1000;
    fprintf(stdout, "Execution time: %'lu us\n", delta_us);
    transfer_rate = \
      (uint64_t)data_chunk_len*8/((double)delta_us/1000000);
    fprintf(stdout, "Transfer Rate: %'lu bps\n", transfer_rate);

    outf_fp = fopen(pdo->outf_name, "a");
    fprintf(
      outf_fp, "%lu,%zu,%lu,%lu\n", iter+1, data_chunk_len, delta_us, transfer_rate);
    fclose(outf_fp);

    if (pdo->demon_mode) iter = 0;
  } /* for (iter=0; iter<dtc->entries->len; ++iter) */

  tfree(data_chunk);

  /* Cleaning up */
  fprintf(stdout, "Closing server...\n");
  zmq_close(data_publisher);
  zmq_ctx_destroy(context);
  tfree(server_addr);
  tfree(status_str);

  fprintf(stdout, "Cleaning up data...\n");
  DeleteDataContainer(dtc);

  return 0;
}

/*************************************
  The server routine - multithreaded - deprecated, Wrong!! approach.
**************************************/
/* Worker argument */
typedef struct _psDAC_mt_worker {
  unsigned char* seg_data_array;
  uint64_t seg_data_len;
  char* address;
  char* inproc_address;
  bool verbose;
  int rc;
  //void* context;
} psDAC_mt_worker;
typedef psDAC_mt_worker* mtWorkerArgs;

/* Worker argument methods */
static mtWorkerArgs NewmtWorkerArgs(
  unsigned char* seg_data_array,
  uint64_t seg_data_len,
  const char* address,
  const char* inproc_address,
  bool verbose)
  //void* context)
{
  mtWorkerArgs wa = (mtWorkerArgs)tmalloc(sizeof(psDAC_mt_worker));
  wa->seg_data_array = seg_data_array;
  wa->seg_data_len = seg_data_len;
  wa->address = strdup(address);
  wa->inproc_address = strdup(inproc_address);
  wa->verbose = verbose;
  //wa->context = context;
  wa->rc = 0;
  return wa;
}

static int DeleteNewmtWorkerArgs(mtWorkerArgs mwa)
{
  assert(mwa);
  tfree(mwa->address);
  tfree(mwa->inproc_address);
  tfree(mwa);
  return 0;
}

static int mtwError(
  const char* error_msg,
  mtWorkerArgs mwa,
  pid_t pid,
  int error_code)
{
  assert(mwa);

  fprintf(stderr, "[pid:%d] %s failed with code [%d]...\n", pid, error_msg, error_code);
  fprintf(stderr, "[pid:%d] Data len: %lu\n", pid, mwa->seg_data_len);
  fprintf(stderr, "[pid:%d] Address: %s\n", pid, mwa->address);
  fprintf(stderr, "[pid:%d] Inproc Address: %s\n", pid, mwa->inproc_address);
  fprintf(stderr, "[pid:%d] Data: %s\n", pid, mwa->seg_data_array);

  return error_code;
}

/* The Worker */
static void* psDAC_worker(void* args)
{
  assert(args);
  mtWorkerArgs mwa = (mtWorkerArgs)pth_args_get_data((pth_args)args);
  if (!mwa) return NULL;
  pid_t pid = pth_args_get_pid(args);

  // void* context = mwa->context;
  void* context = zmq_ctx_new();
  void* socket = zmq_socket(context, ZMQ_PUSH);
  if (!socket) {
    mtwError("zmq_socket assignment", mwa, pid, -1);
    exit(-1);
  }
  // mwa->rc = zmq_bind(socket, mwa->address);
  // if (mwa->rc!=0) {
  //   mtwError("zmq_bind", mwa, pid, mwa->rc);
  //   exit(-1);
  // }
  mwa->rc = zmq_connect(socket, mwa->inproc_address);
  if (mwa->rc!=0) {
    mtwError("zmq_connect", mwa, pid, mwa->rc);
    exit(-1);
  }
  zmq_msg_t msg;

  mwa->rc = zmq_msg_init_size(&msg, mwa->seg_data_len);
  memcpy(zmq_msg_data(&msg), mwa->seg_data_array, mwa->seg_data_len);
  if (mwa->rc) {
    mtwError("zmq_msg_init_size", mwa, pid, mwa->rc);
    exit(-1);
  }
  mwa->rc = zmq_send(socket, &msg, mwa->seg_data_len, 0);
  if (mwa->rc!=mwa->seg_data_len) {
    mtwError("zmq_send", mwa, pid, mwa->rc);
    exit(-1);
  }
  mwa->rc = zmq_msg_close(&msg);
  if (mwa->rc==-1) {
    mtwError("zmq_msg_close", mwa, pid, mwa->rc);
    exit(-1);
  }

  zmq_close(socket);
  zmq_ctx_destroy(context);

  return NULL;
}

/* Data assignment for multithreading */
static unsigned char*** DataAssign(
  List data_list, uint64_t n_threads, uint64_t* n_iter)
{
  unsigned char*** segmented_arrays;
  uint64_t total_data_elements = LLen(data_list);
  uint64_t n_data_segments = total_data_elements/n_threads;
  uint64_t n_data_per_thread_rem = total_data_elements%n_threads;
  uint64_t curr_iter = n_threads;
  uint64_t i, j;

  if (n_data_per_thread_rem) n_data_segments++;

  segmented_arrays = \
    (unsigned char***)tmalloc(sizeof(unsigned char**)*n_data_segments);
  for (i=0; i<n_data_segments; ++i) {
    if (n_data_per_thread_rem && i==n_data_segments-1)
      curr_iter = n_data_per_thread_rem;
    segmented_arrays[i] = \
      (unsigned char**)tmalloc(sizeof(unsigned char*)*curr_iter);
    for (j=0; j<curr_iter; ++j)
      segmented_arrays[i][j] = \
        (unsigned char*)LAtSeq(data_list, j+i*n_threads);
  } /* for (i=0; i<n_data_segments; ++i) */
  LResetCursor(data_list);
  (*n_iter) = n_data_segments;
  return segmented_arrays;
}

/* free thrd_data */
static void free_thread_data(
  unsigned char*** d, uint64_t d_len)
{
  assert(d);
  uint64_t i;
  for (i=0; i<d_len; ++i) {
    tfree(d[i]);
  }
  tfree(d);
}

/* Assign data length */
static uint64_t*** DataLenAssign(
  List data_len_list, uint64_t n_threads, uint64_t* n_iter)
{
  uint64_t*** segmented_arrays;
  uint64_t total_data_elements = LLen(data_len_list);
  uint64_t n_data_segments = total_data_elements/n_threads;
  uint64_t n_data_per_thread_rem = total_data_elements%n_threads;
  uint64_t curr_iter = n_threads;
  uint64_t i, j;

  if (n_data_per_thread_rem) n_data_segments++;

  segmented_arrays = \
    (uint64_t***)tmalloc(sizeof(uint64_t**)*n_data_segments);
  for (i=0; i<n_data_segments; ++i) {
    if (n_data_per_thread_rem && i==n_data_segments-1)
      curr_iter = n_data_per_thread_rem;
    segmented_arrays[i] = \
      (uint64_t**)tmalloc(sizeof(uint64_t*)*curr_iter);
    for (j=0; j<curr_iter; ++j) {
      segmented_arrays[i][j] = \
        (uint64_t*)LAtSeq(data_len_list, j+i*n_threads);
    }
  } /* for (i=0; i<n_data_segments; ++i) */
  LResetCursor(data_len_list);
  (*n_iter) = n_data_segments;
  return segmented_arrays;
}

static void free_thread_data_len(uint64_t*** dl, uint64_t dl_len)
{
  assert(dl);
  uint64_t i;
  for (i=0; i<dl_len; ++i)
    tfree(dl[i]);
  tfree(dl);
}

/* Server caller - multithreaded: Deprecated! Wrong approach!! */
int run_psDAC_mt(psDAC_Options pdo)
{
  assert(pdo);

  if (!pdo) {
    fprintf(stderr, "Invalid options!!\n");
    return -1;
  }

  if (!(pdo->port_number >= MIN_PORT && pdo->port_number <= MAX_PORT)) {
    fprintf(
      stderr,
      "Port number must be within %d - %d range\n",
      MIN_PORT, MAX_PORT);
    return -1;
  }

  char* server_addr;
  int server_addr_str_len;
  char* status_str = status_report(pdo);
  uint64_t i, thrd_iter, iter;
  uint64_t delta_us, transfer_rate;
  struct timespec start, end;

  fprintf(stdout, "%s\n", status_str);

  server_addr_str_len = snprintf(NULL, 0, "tcp://*:%d", pdo->port_number);
  server_addr = (char*)tmalloc(sizeof(char)*(server_addr_str_len+1));
  sprintf(server_addr, "tcp://*:%d", pdo->port_number);

  fprintf(stdout, "Running Pseudo DAC server on %s\n", server_addr);
  fprintf(stdout, "Preparing data from %s\n", pdo->data_file);
  DataContainer dtc = NewDataContainer(pdo->data_file);
  fprintf(stdout, "Data has been prepared!!\n");
  PrintDataContainer(dtc);

  uint64_t n_thread_iter, n_data_len_iter;
  uint64_t curr_threads = pdo->n_threads;
  fprintf(stdout, "Preparing data for multithreaded operation\n");
  unsigned char*** splitted_data = \
    DataAssign(dtc->entries, pdo->n_threads, &n_thread_iter);
  uint64_t*** splitted_data_len = \
    DataLenAssign(dtc->entry_len, pdo->n_threads, &n_data_len_iter);

  fprintf(stdout, "Running the multithreaded operation.\n");
  /* Preparing output file */
  FILE *outf_fp;
  /* prepare output file header */
  outf_fp = fopen(pdo->outf_name, "w");
  fprintf(outf_fp, "%s\n", status_str);
  fprintf(outf_fp, FILE_HEADER);
  fclose(outf_fp);

  /* Preparing zmq stuff */
  void* context = zmq_ctx_new();
  void* workers = zmq_socket(context, ZMQ_PULL);
  zmq_bind(workers, INPROC_ADDRESS);
  void* publish = zmq_socket(context, ZMQ_PUB);
  zmq_bind(publish, server_addr);

  /* Preparing pthread stuff */
  Threads psDAC_thrds;
  mtWorkerArgs* w_args;
  uint64_t rem = LLen(dtc->entries)%pdo->n_threads;
  uint64_t seg_len;
  unsigned char* data_str_from_workers;

  for (iter=0; iter<pdo->iteration; ++iter) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (thrd_iter=0; thrd_iter<n_thread_iter; ++thrd_iter) {
      if (rem!=0 && thrd_iter==n_thread_iter-1) curr_threads = rem;

      w_args = (mtWorkerArgs*)tmalloc(sizeof(mtWorkerArgs)*curr_threads);
      for (i=0; i<curr_threads; ++i) {
        w_args[i] = NewmtWorkerArgs(
          splitted_data[thrd_iter][i],
          *splitted_data_len[thrd_iter][i],
          server_addr, INPROC_ADDRESS,
          pdo->verbose);
        seg_len += *splitted_data_len[thrd_iter][i];
      } /* for (i=0; i<curr_threads; ++i) */

      psDAC_thrds = NewThreads(curr_threads, true, NULL);
      RunThreads(psDAC_thrds, &psDAC_worker, (void**)w_args);
      data_str_from_workers = \
        (unsigned char*)tmalloc(sizeof(unsigned char)*seg_len);
      zmq_recv(workers, data_str_from_workers, seg_len, 0);
      zmq_send(publish, data_str_from_workers, seg_len, 0);
      tfree(data_str_from_workers);

      DeleteThreads(psDAC_thrds);
      for (i=0; i<curr_threads; ++i) DeleteNewmtWorkerArgs(w_args[i]);
      tfree(w_args);
    } /* for (thrd_iter=0; thrd_iter<n_thread_iter; ++thrd_iter) */

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    fprintf(stdout, "[%lu/%lu] Iteration finished!!\n", iter+1, pdo->iteration);
    fprintf(stdout, "\n");

    delta_us = (end.tv_sec-start.tv_sec)*1000000+(end.tv_nsec-start.tv_nsec)/1000;
    fprintf(stdout, "Execution time: %'lu us\n", delta_us);
    transfer_rate = (uint64_t)dtc->entries->len*8/((double)delta_us/1000000);
    fprintf(stdout, "Transfer Rate: %'lu bps\n", transfer_rate);

    outf_fp = fopen(pdo->outf_name, "a");
    fprintf(outf_fp, "%lu,%zu,%lu,%lu\n", iter+1, seg_len, delta_us, transfer_rate);
    fclose(outf_fp);

  } /* for (iter=0; iter<pdo->iteration; ++iter) */

  /* Cleaning up */
  free_thread_data(splitted_data, n_thread_iter);
  free_thread_data_len(splitted_data_len, n_thread_iter);

  zmq_close(publish);
  zmq_close(workers);
  zmq_ctx_destroy(context);

  fprintf(stdout, "Cleaning up data...\n");
  DeleteDataContainer(dtc);

  return 0;
}

/*************************************
  The main function
**************************************/
int main (int argc, char* argv[])
{
  int rc;

  setlocale(LC_NUMERIC, "en_US.utf-8");

  psDAC_Options options = NewpsDAC_Options(argc, argv);

  fprintf(stdout, "*****************************************************\n");
  fprintf(stdout, "***** Pseudo DAC Emulator for VIPIC data server *****\n");
  fprintf(stdout, "***** >>> Version %d.%d.%d                     <<< *****\n",
    VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  fprintf(stdout, "*****************************************************\n");
  fprintf(stdout, "\n");

  if (!options->chunk_mode) rc = run_psDAC(options);
  else rc = run_psDAC_chunk(options);

  // if (options->n_threads <= 1) {
  //   rc = run_psDAC(options);
  // }
  // else
  //   rc = run_psDAC_mt(options);

  if (rc) {
    fprintf(stderr, "Something went wrong with the server operation!!\n");
    return rc;
  }
  else fprintf(stdout, "psDAC finished.\n");

  DeletepsDAC_Options(options);

  return 0;
}
