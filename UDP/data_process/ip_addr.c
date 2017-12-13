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
  IP_Table new_ipt = (IP_Table)tmalloc(sizeof(ip_tbl));
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
  assert(addr);

  if (port >= 65535 || port < 1000)
    ERROR("IPTPush() Port range check!!")

  LPush(ipt->addresses, addr);
  NLPush(ipt->ports, port);
  ipt->size++;
  return;
}

void IPTPushAddr(IP_Table ipt, const char* faddr)
{
  assert(ipt);
  assert(faddr);

  char* addr;
  int port;
  dt_IPAddrparse(faddr, &addr, &port);
  IPTPush(ipt, addr, port);

  tfree(addr);
  return;
}

void IPTPushAddrPortRngStr(IP_Table ipt, const char* faddr_ptrng)
{
  assert(ipt);
  assert(faddr_ptrng);

  char *addr;
  int start_pt, end_pt, i;

  dt_IPAddrportparse(faddr_ptrng, &addr, &start_pt, &end_pt);
  assert(*addr);

  for (i=0; i<end_pt-start_pt+1; ++i)
    IPTPush(ipt, addr, start_pt+i);

  tfree(addr);
  return;
}

void IPTPushPortRngStr(IP_Table ipt, const char* addr, const char* port_rng)
{
  assert(ipt);
  assert(addr);
  assert(port_rng);

  char *st_port, *end_port;
  dt_portparse(port_rng, &st_port, &end_port);
  IPTPushPortRng(ipt, addr, atoi(st_port), atoi(end_port));

  tfree(st_port);
  tfree(end_port);

  return;
}

void IPTPushPortRng(IP_Table ipt, const char* addr, int st_port, int end_port)
{
  assert(addr);
  assert(ipt);
  assert(end_port-st_port>=0);

  if (end_port-st_port == 0) {
    IPTPush(ipt, addr, st_port);
    return;
  }

  int i, n_ports = end_port-st_port+1;

  for (i=0; i<n_ports; ++i) {
    IPTPush(ipt, addr, st_port+1);
  }

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
/* Some other utilities */
/* IPAddr... or Full IP address (xxx.xxx.xxx.xxx:pppp) parser */
void dt_IPAddrparse(const char* FAddr, char** Addr, int* port)
{
  assert(FAddr);

  char* pt_str;
  if ( str_div(FAddr, Addr, &pt_str, ':') == 1)
    mprintf(
      "dt_IPAddrparse: Check up given FAddr.\n"
      "It should be... XXX.XXX.XXX.XXX:XXXXX\n"
    );

  *port = atoi(pt_str);

  return;
}

/* Port range parse */
/* i.e. 10000-10020 to 10000 and 10020 */
void dt_portparse(const char* PTrng, char** start_pt, char** end_pt)
{
  assert(PTrng);

  if (str_div(PTrng, start_pt, end_pt, '-') == 1) {
    mprintf(
      "dt_portparse: Check up given port range.\n"
      "It should be XXXX-XXXX format.\n");
    *start_pt = NULL;
    *end_pt = NULL;
  }

  return;
}

/* IP Addr and port parse */
void dt_IPAddrportparse(const char* FAddr_portrng, char** Addr, int* start_pt, int* end_pt)
{
  assert(FAddr_portrng);

  char* pt_rng_str;
  if (str_div(FAddr_portrng, Addr, &pt_rng_str, ':') == 1) {
    mprintf(
      "dt_IPAddrportparse: Check up the given format!!\n"
      "Format should be..\n"
      "XXX.XXX.XXX.XXX:XXXXX-XXXXX\n"
    );
    *Addr = NULL;
    *start_pt = 0;
    *end_pt = 0;
    return;
  }

  assert(pt_rng_str);

  char *st_pt_str, *end_pt_str;
  if (str_div(pt_rng_str, &st_pt_str, &end_pt_str, '-') == 1) {
    st_pt_str = NULL;
    end_pt_str = NULL;
    return;
  }

  *start_pt = atoi(st_pt_str);
  *end_pt = atoi(end_pt_str);

  return;
}

/* General string divider */
int str_div(const char* orig_str, char** fwd_pt, char** aft_pt, char delim)
{
  assert(orig_str);
  assert(fwd_pt); assert(aft_pt);

  char* p_delim = strchr(orig_str, delim);
  if (*p_delim == '\0') {
    mprintf("str_div: Can't find delimiter in the given string.\n");
    *fwd_pt = NULL; *aft_pt = NULL;
    return 1;
  }

  int fwd_pt_len = p_delim - orig_str;
  *fwd_pt = (char*)tmalloc(fwd_pt_len+1);
  assert(*fwd_pt);
  strncpy(*fwd_pt, orig_str, fwd_pt_len);

  int aft_pt_len = strlen(orig_str) - fwd_pt_len;
  *aft_pt = (char*)tmalloc(aft_pt_len+1);
  assert(*aft_pt);
  strncpy(*aft_pt, orig_str+fwd_pt_len+1, aft_pt_len);

  return 0;
}
