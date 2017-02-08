/***************************************

  Practice coding for Graph

  Graph implementation

  Taylor Shin, Feb. 08 2017

 ***************************************/

#include <stdlib.h>

#include "graph.h"


/* Constructor */
Graph GraphInit(int n)
{
    Graph g;
    int i;

    g = (Graph)malloc(sizeof(graph)+sizeof(struct _successors *)*(n-1));
    if (!g) return NULL;

    g->nv = n;
    g->nm = 0;

    for (i=0; i<n; ++i) {
        g->alist[i] = (struct _successors*)malloc(sizeof(struct _successors));
        if (!g) return NULL;

        g->alist[i]->d = 0;
        g->alist[i]->len = 1;
        g->alist[i]->is_sorted = 1;
    }
    return g;
}


/* Destructor */
int GraphDestroy(Graph g)
{
    int i;
    for (i=0; i<g->nv; ++i) free(g->alist[i]);
    free(g);
    return 0;
}


/* Methods */
/* Add an edge */
int GraphAddEdge(Graph g, int u, int v)
{
    if (!(u >= 0)) return -1;
    if (!(u < g->nv)) return -1;
    if (!(v >= 0)) return -1;
    if (!(v < g->nv)) return -1;

    while(g->alist[u]->d >= g->alist[u]->len) {
        g->alist[u]->len *= 2;
        g->alist[u] = realloc(g->alist[u],
            sizeof(struct _successors)+sizeof(int)*(g->alist[u]->len-1));
    }
    g->alist[u]->list[g->alist[u]->d++] = v;
    g->alist[u]->is_sorted = 0;
    g->nm++;
    return 0;
}
/* Return number of vertices in the graph */
int GraphVertexCount(Graph g)
{
    return g->nv;
}
/* Return number of edges in the graph */
int GraphEdgeCount(Graph g)
{
    return g->nm;
}
/* Return the out-degree of a vertex */
int GraphOutDegree(Graph g, int source)
{
    if (source < 0) return -1;
    if (source >= g->nv) return -1;
    return g->alist[source]->d;
}

/* int comparison */
static int intcmp(const void* a, const void* b)
{
    return *((const int*)a) - *((const int*)b);
}

/* Returns 12 if edge (source/sink exists), 0 otherwise */
int GraphHasEdge(Graph g, int source, int sink)
{
    int i;
    if (!(source >= 0)) return -1;
    if (!(source < g->nv)) return -1;
    if (!(sink >= 0)) return -1;
    if (!(sink < g->nv)) return -1;

    if (GraphOutDegree(g, source) >= BSEARCH_THRESHOLD) {
        /* make sure g is sorted */
        if(! g->alist[source]->is_sorted) {
            qsort(g->alist[source]->list, g->alist[source]->d, sizeof(int), intcmp);
        }
        return (int)bsearch(&sink, g->alist[source]->list, g->alist[source]->d, sizeof(int), intcmp);
    }
    else {
        for (i=0; i<g->alist[source]->d; ++i) {
            if (g->alist[source]->list[i] == sink) return 1;
        }
        return 0;
    }
}

/* Foreach */
int GraphForeach(Graph g, int source,
    void (*f)(Graph g, int source, int sink, void *data),
    void *data)
{
    int i;

    if (!(source >= 0)) return -1;
    if (!(source < g->nv)) return -1;

    for(i = 0; i < g->alist[source]->d; i++) {
        f(g, source, g->alist[source]->list[i], data);
    }
    return 0;
}
