/***************************************

  Practice coding for search (with Graph)

  header file

  Taylor Shin, Feb. 09 2017

  reference:
  http://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Graphs.html

 ***************************************/

#ifndef __SEARCH_H__
#define __SEARCH_H__

/* Typical usage:
 *
 *    SearchInfo s;
 *    int n;
 *
 *    s = SearchInfoInit(g);
 *
 *    n = GraphVertices(g);
 *    for(i = 0; i < n; i++) {
 *        SearchDFS(s, i);
 *    }
 *
 *    ... use results in s ...
 *
 *    SearchInfoDestroy(s);
 *
 */

#include "graph.h"

#define SEARCH_INFO_NULL -1

typedef struct _search_info {
	Graph graph;
	int reached;   /* count of reached nodes */
	int *preorder; /* list of nodes in order first reached */
	int *time;     /* time[i] is position of node i in preorder list */
	int *parent;   /* parent in DFS or BFS forest */
	int *depth;    /* distance from root */
} search_info;
typedef search_info* SearchInfo;

/* Constructors */
SearchInfo SearchInfoInit(Graph g);
/* Destructors */
int SearchInfoDestroy(SearchInfo s);
/* Depth-first search from root */
int SearchDFS(SearchInfo results, int root);
/* Breadth-first search from root */
int SearchBFS(SearchInfo results, int root);

/* some local utilities */
static int* CreateEmptyArray(int n);

typedef struct _edge {
	int u; int v; /* source and sink */
} edge;
typedef edge* Edge;

typedef struct _queue {
	Edge e;
	int bottom;
	int top;
} queue;
typedef queue Queue;

static int Push(Graph g, int u, int v, void* data);
static int Search(SearchInfo r, int root, int use_queue);

#endif /* Include guard */
