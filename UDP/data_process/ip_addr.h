/*************************************************
  A simple UDP random packet generator

  Purpose
  To emulate massive data influx from the
  VIPIC board.

  --> IP Address string handler

  Writen by Taylor Shin

  Dec. 11th 2017

**************************************************/
#ifndef DATA_PROCESS_IPADDR_HANDLER_HEADER_FILE
#define DATA_PROCESS_IPADDR_HANDLER_HEADER_FILE

#include "utils.h"

/* Some data structure definitions */
typedef struct _simpl_linked_list sllist;
typedef sllist* List;
typedef struct _simpl_linked_list_node sllnode;
typedef sllnode* LNode;
typedef struct _simpl_linked_list_num sllist_n;
typedef sllist_n* NList;
typedef struct _simpl_linked_list_node_num sllnode_n;
typedef sllnode_n* NNode;

/* Some methods for linked list */
List NewList();
void DeleteList(List l);
LNode NewNode(const char* text);
void DeleteNode(LNode node);
NList NewNList();
void DeleteNList(NList l);
NNode NewNNode(int num);
void DeleteNNode(NNode node);

/* Linked list manipulation methods */
void LPush(List l, const char* text);
char* LPop(List l);
char* LAt(List l, uint64_t index);
void NLPush(NList l, int num);
int NLPop(NList l);
int NLAt(NList l, uint64_t index);

/* Data Structure for address list */
typedef struct _ip_tbl ip_tbl;
typedef ip_tbl* IP_Table;

/* Some methods for address list */
IP_Table NewIP_Table();
void DeleteIP_Table(IP_Table ipt);
void IPTPush(IP_Table ipt, const char* addr, int port);
void IPTPushAddr(IP_Table ipt, const char* faddr);
void IPTPushAddrPortRngStr(IP_Table ipt, const char* faddr_ptrng);
void IPTPushPortRngStr(IP_Table ipt, const char* addr, const char* port_rng);
void IPTPushPortRng(IP_Table ipt, const char* addr, int st_port, int end_port);
char* IPTAddrAt(IP_Table ipt, uint64_t index);
int IPTPortAt(IP_Table ipt, uint64_t index);
uint64_t IPTMaxPort(IP_Table ipt);
uint64_t IPTGetSz(IP_Table ipt);

/* Some other utilities */
/* IPAddr... or Full IP address (xxx.xxx.xxx.xxx:pppp) parser */
void dt_IPAddrparse(const char* FAddr, char** Addr, int* port);

/* Port range parse */
/* i.e. 10000-10020 to 10000 and 10020 */
void dt_portparse(const char* PTrng, char** start_pt, char** end_port);

/* IP Addr and port parse */
void dt_IPAddrportparse(const char* FAddr_portrng, char** Addr, int* start_pt, int* end_pt);

/* General string divider */
int str_div(const char* orig_str, char** fwd_pt, char** aft_pt, char delim);

#endif /* Include guard */
