/***************************************

  Practice coding for Graph

  Graph header file

  Taylor Shin, Feb. 08 2017

  Reference:
  http://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Graphs.html

 ***************************************/
 #ifndef __GRAPH_H__
 #define __GRAPH_H__

#define BSEARCH_THRESHOLD 10

typedef struct _graph {
    int nv; /* number of vertices */
    int nm; /* number of edges */

    struct _successors {
        int d; /* number of successors */
        int len; /* number of slots in array */
        short is_sorted; /* 1 if list is sorted */
        int list[1]; /* List of successors */
    } *alist[1];
} graph;

typedef graph* Graph;

/* Constructor */
Graph GraphInit(int n);
/* Destructor */
int GraphDestroy(Graph g);

/* Methods */
int GraphAddEdge(Graph g, int u, int v);
int GraphVertexCount(Graph g);
int GraphEdgeCount(Graph g);
int GraphOutDegree(Graph g, int source);
int GraphHasEdge(Graph g, int source, int sink);

static int intcmp(const void* a, const void* b);
int GraphForeach(
    Graph g, int source,
    void (*f)(Graph g, int source, int sink, void *data),
    void *data);

 #endif /* Include guard */
