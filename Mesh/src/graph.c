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
#include <stdint.h>

#include "btree.h"
#include "graph.h"

/* Some static methods */
static int IsRoot(GNode r);
static unsigned int Keygen(btree_data_t data);



/* Constructors and Destructors */
/* Control node */
/* Regular matrix */
/* Constructor */
Graph NewGraph()
{
    Graph g = (Graph)malloc(sizeof(graph_root));
    assert(g);

    g->root = InitGraph();
    g->vertices = 0;
    g->edges = 0;
    g->adjMatrix = InitMatrix(1, 1);

    return g;
}

/* Destructor */
int DeleteGraph(Graph g)
{
    assert(g);

    if (g->root) GraphDestroy(g->root);
    if (g->adjMatrix) DestroyMatrix(g->adjMatrix);

    return 0;
}

/* Sparse matrix */
/* Constructor */
SGraph NewSGraph()
{
    SGraph g = (SGraph)malloc(sizeof(graph_root_s));
    assert(g);

    g->root = InitGraph();
    g->vertices = 0;
    g->edges = 0;
    g->adjMatrix = InitSMatrix(1, 1);

    return g;
}
/* Destructor */
int DeleteSGraph(SGraph g)
{
    assert(g);

    if (g->root) GraphDestroy(g->root);
    if (g->adjMatrix) DestroySMatrix(g->adjMatrix);

    return 0;
}

/* for nodes */
/* Constructors */
GNode InitGraph()
{
    GNode gn = (GNode)malloc(sizeof(graph_node)); assert(gn);
    assert(gn);
    LNode lnks = ListInit();
    LNode prvs = ListInit();

    gn->data = NULL;
    gn->links = lnks;
    gn->prevs = prvs;
    gn->index = 0;

    return gn;
}
/* Constructors with data */
GNode InitGraphData(graph_data_t data)
{
    GNode gn = InitGraph();
    gn->data = data;
    gn->index = (unsigned long)data;
    return gn;
}
/* Make graph node with data (alias) */
GNode GraphMakeNode(graph_data_t data)
{
    return InitGraphData(data);
}


/* Destructor */
int GraphDestroy(GNode r)
{
    assert(r);

    GNode tmp = r;
    GNode ntmp;
    BTNode history = InitBT();
    LNode nxt_tmp = tmp->links;
    unsigned long key = Keygen(tmp);

    /* Populate a list of graphs (In fact, a binary tree) */
    while (1) {
        /* If current node isn't included in the history ... */
        if (!BTSearch(history, key))
            BTInsert(history, tmp, key);
        else break;

        /* Now find a suitable next node */
        while (nxt_tmp) {
            if (nxt_tmp->value) {
                ntmp = (GNode)nxt_tmp->value;
                GraphDestroy(ntmp);
                nxt_tmp = nxt_tmp->next;
            }
            else break;
        }
        ListDestroy(tmp->links);
        ListDestroy(tmp->prevs);
        free(tmp);
    }

    /* clean up history */
    BTDestroy(history);

    return 0;
}




/* Manipulatin methods */
int GraphPush(GNode* r, GNode other)
{
    assert((*r));
    assert(other);

    ListPush(&(*r)->prevs, other);
    ListPush(&other->links, (*r));
    (*r) = other;

    return 0;
}
int GraphAttach(GNode r, GNode other)
{
    assert(r);
    assert(other);

    ListPush(&r->links, other);
    ListPush(&other->prevs, r);

    return 0;
}




/* Utilities */
/* Traverse the entire graph and count node numbers */
int GraphTraverse(GNode* r, unsigned long* n_tot)
{
    assert(*r);
    assert(n_tot);

    GNode tmp = (*r);
    GNode ntmp;
    unsigned long num_g = 0;
    BTNode history = InitBT();
    LNode nxt_tmp = tmp->links;

    unsigned long p_num = 0;
    unsigned long p_num_tmp;

    unsigned long key = Keygen(tmp);

    while (1) {
        /* If current node isn't included in the history ... */
        if (!BTSearch(history, key))
            BTInsert(history, tmp, key);
        else break;

        /* Now find a suitable next node */
        while (nxt_tmp) {
            if (nxt_tmp->value) {
                ntmp = (GNode)nxt_tmp->value;
                GraphTraverse(&ntmp, &p_num_tmp);
                p_num += p_num_tmp;
                nxt_tmp = nxt_tmp->next;
            }
            else break;
        }
        //nxt_tmp = tmp->links;
        ++num_g;
        num_g += p_num;
    }

    /* Update node numbers */
    (*n_tot) = num_g;

    /* clean up history */
    BTDestroy(history);

    return 0;
}

/* Find graph node that holds designated data */
int GraphFind(GNode* r, GNode* found, graph_data_t data)
{
    assert(*r);

    if (!IsRoot(*r)) {
        (*found) = NULL;
        return 1;
    }


    return 0;
}



/* Static functions */
/* Check if it's root */
static int IsRoot(GNode r)
{
    assert(r);
    if (ListIsEmpty(r->prevs)) return 0;
    else return 1;
}

/* keygen for binary tree */
static unsigned int Keygen(btree_data_t data)
{
    unsigned int k = 0;
    uintptr_t idata = (uintptr_t)data;
    k = (unsigned int)idata;

    return k;
}
