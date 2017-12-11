/*************************************************
  A simple UDP random packet generator

  Purpose
  To emulate massive data influx from the
  VIPIC board.

  --> IP Address string handler

  Implementation.

  Writen by Taylor Shin

  Dec. 8th 2017

**************************************************/

#include "ip_addr.h"

/* Defining a linked list to store the IP address */
typedef struct _simpl_linked_list {
  struct _simpl_linked_list_node* head;
  uint64_t size;
} sllist;

typedef struct _simpl_linked_list_node {
  struct _simpl_linked_list_node* next;
  char* data;
} sllnode;

/* ------------------------------------------ */
typedef struct _simpl_linked_list_num {
  struct _simpl_linked_list_node_num* head;
  uint64_t size;
} sllist_n;

typedef struct _simpl_linked_list_node_num {
  struct _simpl_linked_list_node_num* next;
  int data;
} sllnode_n;

/* Linked list constructors and destructors */
List NewList()
{
  List new_list = (List)tmalloc(sizeof(sllist));
  assert(new_list);
  new_list->head = NULL;
  new_list->size = 0L;
  return new_list;
}

void DeleteList(List l)
{
  assert(l);
  uint64_t i;
  char* tmp_txt;
  if (l->size) {
    for (i=0; i<l->size; ++i) {
      tmp_txt = LPop(l);
      tfree(tmp_txt);
    }
  }
  tfree(l);
  return;
}

LNode NewNode(const char* text)
{
  LNode new_node = tmalloc(sizeof(sllnode));
  assert(new_node);
  new_node->next = NULL;

  if (text) {
    new_node->data = (char*)tmalloc(strlen(text)+1);
    assert(new_node->data);
    strcpy(new_node->data, text);
  }
  else new_node->data = NULL;

  return new_node;
}

void DeleteNode(LNode node)
{
  assert(node);
  tfree(node->data);
  tfree(node);
  return;
}

/* ------------------------------------------ */
NList NewNList()
{
  NList new_list = (NList)tmalloc(sizeof(sllist_n));
  assert(new_list);
  new_list->head = NULL;
  new_list->size = 0L;
  return new_list;
}

void DeleteNList(NList l)
{
  assert(l);
  uint64_t i;
  if (l->size)
    for (i=0; i<l->size; ++i) NLPop(l);
  tfree(l);
  return;
}

NNode NewNNode(int num)
{
  NNode new_node = tmalloc(sizeof(sllnode_n));
  assert(new_node);
  new_node->next = NULL;
  new_node->data = num;
  return new_node;
}

void DeleteNNode(NNode node)
{
  assert(node);
  tfree(node);
  return;
}



/* Linked list mainpulation methods */
void LPush(List l, const char* text)
{
  assert(l);
  LNode new_node = NewNode(text);

  if (l->head) {
    new_node->next = l->head;
    l->head = new_node;
  }
  else l->head = new_node;
  l->size++;
  return;
}

char* LPop(List l)
{
  assert(l);
  if (!l->head && !l->size) return NULL;

  LNode tmp = l->head;
  l->head = tmp->next;

  char* tmp_str = (char*)tmalloc(strlen(tmp->data)+1);
  assert(tmp_str);
  strcpy(tmp_str, tmp->data);
  DeleteNode(tmp);

  l->size--;

  return tmp_str;
}

char* LAt(List l, uint64_t index)
{
  assert(l);
  if (index >= l->size) return NULL;
  LNode tmp = l->head;
  uint64_t i;
  for (i=0; i<index; ++i) tmp = tmp->next;
  return tmp->data;
}

/* ------------------------------------------ */
void NLPush(NList l, int num)
{
  assert(l);
  NNode new_node = NewNNode(num);

  if (l->head) {
    new_node->next = l->head;
    l->head = new_node;
  }
  else l->head = new_node;
  l->size++;
  return;
}

int NLPop(NList l)
{
  assert(l);
  if (!l->head && !l->size) return 0;

  NNode tmp = l->head;
  l->head = tmp->next;

  int tmp_data = tmp->data;
  DeleteNNode(tmp);

  l->size--;

  return tmp_data;
}

int NLAt(NList l, uint64_t index)
{
  assert(l);
  if (index >= l->size) return 0;
  NNode tmp = l->head;
  uint64_t i;
  for (i=0; i<index; ++i) tmp = tmp->next;
  return tmp->data;
}

/**********************************************
  Data structure for IP Address storage
***********************************************/
typedef struct _ip_tbl {
  List addresses;
  NList ports;
  uint64_t size;
} ip_tbl;

/* Methods for IP Table */
IP_Table NewIP_Table()
{
  IP_Table new_ipt = \
    (IP_Table)tmalloc(sizeof(ip_tbl));
  assert(new_ipt);
  new_ipt->addresses = NewList();
  new_ipt->ports = NewNList();
  new_ipt->size = 0L;
  return new_ipt;
}

void DeleteIP_Table(IP_Table ipt)
{
  assert(ipt);
  DeleteList(ipt->addresses);
  DeleteNList(ipt->ports);
  tfree(ipt);
  return;
}

void IPTPush(IP_Table ipt, const char* addr, int port)
{
  assert(ipt);
  LPush(ipt->addresses, addr);
  NLPush(ipt->ports, port);
  ipt->size++;
  return;
}

void IPTPushAddr(IP_Table ipt, const char* faddr)
{
  char* addr;
  int port;
  dt_urlparse(faddr, &addr, &port);
  IPTPush(ipt, addr, port);
  tfree(addr);
  return;
}

char* IPTAddrAt(IP_Table ipt, uint64_t index)
{
  assert(ipt);
  return LAt(ipt->addresses, index);
}

int IPTPortAt(IP_Table ipt, uint64_t index)
{
  assert(ipt);
  return NLAt(ipt->ports, index);
}

uint64_t IPTMaxPort(IP_Table ipt)
{
  assert(ipt);
  assert(ipt->ports);
  uint64_t max_ind = 0L;
  uint64_t i;
  NNode tmp;

  tmp = ipt->ports->head;
  for (i=1; i<ipt->size; ++i) {

    max_ind = \
      (NLAt(ipt->ports, i-1) > NLAt(ipt->ports, i)) ? (i-1) : i;

    tmp = tmp->next;

  } /* for (i=1; i<ipt->size; ++i) */

  return max_ind;
}

uint64_t IPTGetSz(IP_Table ipt)
{
  assert(ipt);
  return ipt->size;
}

/**********************************************
  Utilities
***********************************************/
/* URL parser */
void dt_urlparse(const char* FAddr, char** Addr, int* port)
{
  assert(FAddr);
  char* colon = strchr(FAddr, ':');
  int ip_length = colon - FAddr;
  *Addr = (char*)tmalloc(ip_length+1);
  strncpy(*Addr, FAddr, ip_length);
  int port_length = strlen(FAddr) - ip_length;
  char port_str[port_length];
  strncpy(port_str, FAddr+ip_length+1, port_length);
  *port = atoi(port_str);

  return;
}
