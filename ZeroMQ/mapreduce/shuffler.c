/****************************************

  Another futile attempt to implement
  Mapreduce... with ZeroMQ this time.

  Shuffler implementation

  Implementation file

  Written by Taylor Shin
  May 9th 2017

****************************************/
#include "shuffler.h"

/****************************************
  Shuffler - Some static stuffs
****************************************/
static void PrintHelp()
{
  printf(
    "\nShuffler for mapreduce_0MQ...\n\n"
    "Usage: shuffler <options> <parameters>\n"
    "Options:\n"
    "-a\tAddress of the shuffler to be bound. Default: %s\n"
    "-m\tList of mapper addresses delimited by ;\n"
    "-n\tList of neighboring shuffler addresses, delimited by ;\n"
    "-k\tAssign Key with this command.\n"
    "-h\tPrints this message.\n"
    , DEFAULT_ADDRESS
  );
  printf("\n");
}

static char* char_list_to_str(List lc, const char* delim)
{
  if (!lc) return NULL;
  if (!LLen(lc)) return NULL;
  if (LLen(lc)==1) return strdup((char*)LAt(lc, 0));

  char* ret_str = NULL;
  char* tmp_str;

  uint64_t i = 0;
  uint64_t lc_len = LLen(lc);
  while (i<lc_len) {
    tmp_str = (char*)LAtSeq(lc, i);
    ret_str = append_str(ret_str, tmp_str);
    if (i!=lc_len-1) ret_str = append_str(ret_str, delim);
    ++i;
  } /* while (i<LLen(lc)) */
  LResetCursor(lc);

  return ret_str;
}

static void PrintStatus(Shuffler shfl)
{
  if (!shfl) return;

  printf("=========================================\n"
         "  Shuffler Stats\n"
         "=========================================\n");

  char *f_str_mappers, *f_str_neighbors;
  f_str_mappers = char_list_to_str(shfl->Mappers, "  \n");
  f_str_neighbors = char_list_to_str(shfl->Neighbors, "  \n");

  printf("Address: %s\n", shfl->address);
  printf("Mappers:\n  %s\n",
    f_str_mappers?f_str_mappers:"None");
  printf("Neighboring Shufflers:\n  %s\n",
    f_str_neighbors?f_str_neighbors:"None");
  printf("Assigned Key: %s\n", shfl->assigned_key?shfl->assigned_key:"None");
  printf("=========================================\n\n");

  tfree(f_str_mappers);
  tfree(f_str_neighbors);
}

/* remove some specific characters in a string */
static void remove_all_chars(char* str, char c)
{
  char *pr = str, *pw = str;
  while (*pr) {
    *pw = *pr++;
    pw += (*pw!=c);
  }
  *pw = '\0';
}

/****************************************
  Shuffler - Constructor and Destructor
****************************************/
Shuffler NewShuffler(int argc, char* argv[])
{
  Shuffler shfl = (Shuffler)tmalloc(sizeof(shuffler));

  shfl->Keys = NewList();
  shfl->Data = NewList();

  shfl->address = strdup(DEFAULT_ADDRESS);

  int opt;
  while ( (opt = getopt(argc, argv, "a:m:n:k:h")) != -1 ) {
    switch (opt) {
    case 'a':
      if (shfl->address) tfree(shfl->address);
      shfl->address = strdup(optarg);
      break;
    case 'm':
      if (shfl->mappers) tfree(shfl->mappers);
      if (optarg) {
        shfl->mappers = strdup(optarg);
        shfl->Mappers = ShflParser(shfl->mappers);
      }
      else {
        shfl->mappers = NULL;
        shfl->Mappers = NULL;
      }
      break;
    case 'n':
      if (shfl->neighbors) tfree(shfl->neighbors);
      if (optarg) {
        shfl->neighbors = strdup(optarg);
        shfl->Neighbors = ShflParser(shfl->neighbors);
      }
      else {
        shfl->neighbors = NULL;
        shfl->Neighbors = NULL;
      }
      break;
    case 'k':
      if (optarg) shfl->assigned_key = strdup(optarg);
      else shfl->assigned_key = NULL;
      break;
    case 'h':
      PrintHelp();
      exit(0);
    default:
      PrintHelp();
      exit(0);
    } /* switch (opt) */
  } /* while ( (opt = getopt(argc, argv, "a:h")) != -1 ) */

  return shfl;
}

int DeleteShuffler(Shuffler shfl)
{
  if (!shfl) return -1;

  tfree(shfl->address);
  if (shfl->mappers) tfree(shfl->mappers);
  if (shfl->neighbors) tfree(shfl->neighbors);
  if (shfl->assigned_key) tfree(shfl->assigned_key);
  if (shfl->Mappers) DeleteListHard(shfl->Mappers, NULL);
  if (shfl->Neighbors) DeleteListHard(shfl->Neighbors, NULL);
  DeleteListHard(shfl->Keys, NULL);
  DeleteListHard(shfl->Data, NULL);
  tfree(shfl);

  return 0;
}

/* Other methods */
/* Input parsor for addresses, returns List<char*> */
List ShflParser(const char* addresses)
{
  if (!addresses) ERROR("ShflParser", -1);

  List parsed_list = NewList();
  char* tmp_txt = strdup(addresses);

  /* trimming out some useless characters */
  remove_all_chars(tmp_txt, ' ');

  /* Let's parse */
  char *token;
  char delim[1] = ";";
  while ( ( token = strtok(tmp_txt, delim) ) != NULL )
    LPush(parsed_list, strdup(token));

  tfree(tmp_txt);
  return parsed_list;
}

/****************************************
  Shuffler - Runner
****************************************/
int RunShuffler(Shuffler shfl)
{
  if (!shfl) return -1;

  while (true) {

  }

  return 0;
}

/****************************************
  Shuffler - The main function
****************************************/
int main(int argc, char* argv[])
{
  Shuffler shfl = NewShuffler(argc, argv);
  PrintStatus(shfl);
  RunShuffler(shfl);
  DeleteShuffler(shfl);

  return 0;
}
