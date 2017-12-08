/*************************************************
  A simple UDP random packet generator

  Purpose
  To emulate massive data influx from the
  VIPIC board.

  --> IP Address string handler

  Writen by Taylor Shin

  Dec. 8th 2017

**************************************************/
#ifndef DATA_TOSS_IPADDR_HANDLER_HEADER_FILE
#define DATA_TOSS_IPADDR_HANDLER_HEADER_FILE

#include <assert.h>

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
char* IPTAddrAt(IP_Table ipt, uint64_t index);
int IPTPortAt(IP_Table ipt, uint64_t index);
uint64_t IPTMaxPort(IP_Table ipt);
uint64_t IPTGetSz(IP_Table ipt);

/* Some other utilities */
/* URL... or Full IP address (xxx.xxx.xxx.xxx:pppp) parser */
void dt_urlparse(const char* FAddr, char* Addr, int* port);

#endif /* Include guard */
