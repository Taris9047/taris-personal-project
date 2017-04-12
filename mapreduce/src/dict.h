/**
* @Author: taris
* @Date:   2017-03-22T18:57:27-05:00
* @Last modified by:   taris
* @Last modified time: 2017-03-23T00:08:47-05:00
*/



/***************************************

  Dictionary data structure for C

  Header file

  Taylor Shin, Mar. 22th 2017

 ***************************************/
 #ifndef C_IMPLEMENTATION_DICTIONARY_H
 #define C_IMPLEMENTATION_DICTIONARY_H

#include <stdbool.h>

#include "list.h"
#include "hash.h"
#include "tuple.h"

/* Some hefty typedef */
typedef list_data_t dict_data_t;
typedef unsigned long dict_key_t;

/* Node design */
typedef struct _dict_node {
  dict_data_t data; /* Actual data */
  dict_key_t key;   /* Numeric (or.. hashed) key */
} dict_node;
typedef dict_node* DNode;

/* Node Constructors and Destructors */
DNode NewDNode(dict_data_t inp_data, dict_key_t inp_key);
int DeleteDNode(DNode dn);



/* macro for dict read */
#define tableAt(L, I) (DNode)LAt(L, I)

/* The Dict itself */
typedef struct _dictionary {
  unsigned long long size; /* Size of table */
  /*
    Linked list data structure that holds the nodes
    --> might change to BTree later...
  */
  List table;       /* Actually holds the DNodes with data */
	List key_str;     /* List of key strings List<char*> */
  dict_key_t* keys; /* Array of numeric keys (may not needed?) */
  dict_key_t (*hashing)(); /* Hashing function */

} dictionary;
typedef dictionary* Dict;

/* Dictionary constructors and destructors */
Dict NewDict();
int DeleteDict(Dict d);
int DeleteDictHard(Dict d, int (*data_destroyer) ());

/* Dictionary methods */
/* Setup hashing function */
int DSetHashFunc(Dict d, dict_key_t (*hashing) ());
/* Insert/Get/Remove, typical access stuff */
int DInsert(Dict d, dict_data_t inp_data, const void* inp_key);
dict_data_t DGet(Dict d, const void* inp_key);
int DRemove(Dict d, const void* inp_key);
/* Get list of key strings */
List DGetKeys(Dict d);







/* Providing some time-saving utils */
#define ToStr(T) \
  _Generic( (T), \
    short:sh_to_str, \
    int:i_to_str, \
    unsigned int:ui_to_str, \
    long:l_to_str, \
    unsigned long:ul_to_str, \
    long long:ll_to_str, \
    unsigned long long:ull_to_str, \
    float:f_to_str, \
    double:d_to_str, \
    long double:ld_to_str \
  ) (T)

char* sh_to_str(const short a);
char* i_to_str(const int a);
char* ui_to_str(const unsigned int a);
char* l_to_str(const long a);
char* ul_to_str(const unsigned long a);
char* ll_to_str(const long long a);
char* ull_to_str(const unsigned long long a);
char* f_to_str(const float a);
char* d_to_str(const double a);
char* ld_to_str(const long double a);

#endif /* Include guard */
