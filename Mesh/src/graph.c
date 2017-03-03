/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T08:37:01-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T08:40:06-06:00
*/

/***************************************

  Graph Data Structure for Mesh

  Implementation file

  Taylor Shin, Feb. 21 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "btree.h"
#include "graph.h"
#include "path.h"

/* Some static methods */
//static int IsRoot(GNode r);
//static unsigned int Keygen(btree_data_t data);

//static int Push(GNode* r, GNode other);
static int GNodeCpy(GNode* a, GNode c);
static int Attach(GNode r, GNode other);

static int AdjMExpand(Graph g);
static int AdjMAssign(Graph g, unsigned long key, List keys);

static matrix_data_t MatData(MATRIX_D_T num);
List VerticeList(GNode v[], unsigned long vlen);

/* Constructors and Destructors */
/* Control node */
/* Regular matrix */
/* Constructor */
Graph NewGraph()
{
    Graph g = (Graph)malloc(sizeof(graph_root));
    assert(g);

    g->root_node = init_graph_node();
    g->vertices = 0;
    g->edges = 0;
    g->adjMatrix = InitZeroMatrix(1, 1);

    return g;
}

/* Destructor */
int DeleteGraph(Graph g)
{
    assert(g);

    if (g->root_node) graph_node_destroy(g->root_node);
    if (g->adjMatrix) DestroyMatrix(g->adjMatrix);
    free(g);

    return 0;
}

/* Sparse matrix */
/* Constructor */
SGraph NewSGraph()
{
    SGraph g = (SGraph)malloc(sizeof(graph_root_s));
    assert(g);

    g->root_node = init_graph_node();
    g->vertices = 0;
    g->edges = 0;
    g->adjMatrix = InitSMatrix(1, 1);

    return g;
}
/* Destructor */
int DeleteSGraph(SGraph g)
{
    assert(g);

    if (g->root_node) graph_node_destroy(g->root_node);
    if (g->adjMatrix) DestroySMatrix(g->adjMatrix);
    free(g);

    return 0;
}

/* for nodes */
/* Constructors */
GNode init_graph_node()
{
    GNode gn = (GNode)malloc(sizeof(graph_node)); assert(gn);
    assert(gn);
    LNode lnks = list_node_init();
    LNode prvs = list_node_init();

    gn->data = NULL;
    gn->links = lnks;
    gn->prevs = prvs;
    gn->index = 0;
    gn->edges = 0;

    return gn;
}
/* Constructors with data */
GNode init_graph_node_data(graph_data_t data)
{
    GNode gn = init_graph_node();
    gn->data = data;
    gn->index = (unsigned long)data;
    return gn;
}
/* Make graph node with data (alias) */
GNode graph_make_node(graph_data_t data)
{
    return init_graph_node_data(data);
}


/* Destructor */
int graph_node_destroy(GNode r)
{
    assert(r);

    GNode tmp = r;
    GNode ntmp;
    BTNode history = bt_node_init();
    LNode nxt_tmp = tmp->links;
    unsigned long key = r->index;

    /* Populate a list of graphs (In fact, a binary tree) */
    while (1) {
        /* If current node isn't included in the history ... */
        if (!bt_search(history, key))
            bt_insert(history, tmp, key);
        else break;

        /* Now find a suitable next node */
        while (nxt_tmp) {
            if (nxt_tmp->value) {
                ntmp = (GNode)nxt_tmp->value;
                graph_node_destroy(ntmp);
                nxt_tmp = nxt_tmp->next;
            }
            else break;
        }
        list_node_destroy(tmp->links);
        list_node_destroy(tmp->prevs);
        free(tmp);
    }

    /* clean up history */
    bt_node_destroy(history);

    return 0;
}




/* Manipulatin methods */
/* Attach a node to a graph */
int GraphAttach(Graph g, GNode n, List keys)
{
    assert(g);
    assert(n);
    assert(keys);

    n->index = (g->vertices++);
    /* Update adjmatrix */
    AdjMExpand(g);
    AdjMAssign(g, n->index, keys);
    g->edges = keys->len;

    GNode tmp_gn;
    unsigned long i;
    unsigned long key;
    for (i=0; i<keys->len; ++i) {
        key = ULONGLAt(keys, i);
        graph_node_find(g, &tmp_gn, key);
        Attach(tmp_gn, n);
    }

    return 0;
}

/* Attach to some graph */
int graph_node_attach(GNode n, GNode* adjs, unsigned long n_adjs)
{
    assert(n);

    unsigned long i;
    for (i=0; i<n_adjs; ++i) Attach(adjs[i], n);

    return 0;
}

/* Find node */
int graph_node_find(Graph g, GNode* r, unsigned long key)
{
    assert(g);

    (*r) = NULL;

    int stat;

    GPath g_path = PathInit(g);
    List path = PathFindShort(g_path, key);

    (*r) = GNodeLAt(path, 0);

    stat = DeleteList(path);
    if (stat) return stat;
    stat = DeletePath(g_path);
    if (stat) return stat;

    return 0;
}


/* Node manipulation */
// static int Push(GNode* r, GNode other)
// {
//     assert((*r));
//     assert(other);
//
//     list_node_push(&(*r)->prevs, other);
//     list_node_push(&other->links, (*r));
//
//     (*r) = other;
//
//     return 0;
// }

/* Copy a new GNode */
static int GNodeCpy(GNode* a, GNode c)
{
    assert(c);

    if (*a) graph_node_destroy(*a);

    GNode tmp = init_graph_node();
    tmp->data = c->data;
    list_node_copy(&tmp->links, c->links);
    list_node_copy(&tmp->prevs, c->prevs);
    tmp->index = c->index;
    tmp->edges = c->edges;

    (*a) = tmp;

    return 0;
}

static int Attach(GNode r, GNode other)
{
    assert(r);
    assert(other);

    if (!r->data) return GNodeCpy(&r, other);

    list_node_push(&r->links, other);
    list_node_push(&other->prevs, r);
    r->edges++;

    return 0;
}

/* Adjacent matrix manipulation */
/* expand adjacent matrix for additional graph node */
static int AdjMExpand(Graph g)
{
    assert(g);
    assert(g->adjMatrix);

    g->vertices++;

    return MatrixResize(&g->adjMatrix, g->adjMatrix->rows+1, g->adjMatrix->cols+1);
}

/* Assign new node into adjacent matrix */
/* inputs: the graph itself, new key, and the adjacent key list */
static int AdjMAssign(Graph g, unsigned long key, List keys)
{
    assert(g);
    assert(keys);

    /* the key must be larger than any element in keys at this moment */
    Matrix tmpM = g->adjMatrix;
    matrix_data_t one;
    unsigned long tmp_key;
    unsigned long i;
    for (i=0; i<keys->len; ++i) {
        tmp_key = ULONGLAt(keys, i);
        one = MatData(1);
        MatrixSet(tmpM, tmp_key, key, one);
        MatrixSet(tmpM, key, tmp_key, one);
    }

    return 0;
}

/* Matrix node genration with int */
static matrix_data_t MatData(MATRIX_D_T num)
{
    matrix_data_t d = (matrix_data_t)malloc(sizeof(MATRIX_D_T));
    assert(d);
    (*d) = num;
    return d;
}

/* Make vertice list */
List VerticeList(GNode v[], unsigned long vlen)
{
    List l = NewList();
    unsigned long i;
    for (i=0; i<vlen; ++i) LPush(l, v[i]);
    return l;
}
