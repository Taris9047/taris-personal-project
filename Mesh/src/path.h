/**
* @Author: Taylor Shin
* @Date:   2017-03-03T11:09:45-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   Taylor Shin
* @Last modified time: 2017-03-03T13:12:48-06:00
*/
/***************************************

  Graph Pathfinder

  Header file

  Taylor Shin, Mar. 3 2017

 ***************************************/

#ifndef GRAPH_PATHFINDER_H
#define GRAPH_PATHFINDER_H

#include "graph.h"
#include "matrix.h"
#include "btree.h"
#include "list.h"

/* For convenience */
#define GNodeBTSearch(bt, key) ((GNode)BTSearch(bt, key))

/*
   Path binary tree
   key: weight (unsigned long)
   value: index of graph node (unsigned long)
   --> we need a sorting function
*/

/* Path control node */
typedef struct _graph_path {
    Graph g; /* The graph node */
    BTree path_db; /* path database as a binary tree. */
    int (*path_db_sort) (); /* function pointer for path_db sort */
} graph_path;
typedef graph_path* GPath;

/* Constructors and destructors */
GPath PathInit(Graph g);
int PathDestroy(GPath p);
int DeletePath(GPath p);

/* Methods */
List PathFindShort(GPath p, unsigned long key);
int path_db_sort(BTree path_db);


#endif /* Include guard */
