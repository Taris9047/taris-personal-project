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

/* Some static methods */
int IsRoot(GNode r);


/* Constructors and Destructors */
/* Constructors */
GNode InitGraph()
{
    GNode gn = (GNode)malloc(sizeof(graph_node)); assert(gn);
    LNode lnks = (LNode)malloc(sizeof(list_node)); assert(lnks);
    LNode prvs = (LNode)malloc(sizeof(list_node)); assert(prvs);

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
/* Just an alias for InitGraph() */
GNode NewGraph()
{
    return InitGraph();
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


    return 0;
}




/* Manipulatin methods */
int GraphPush(GNode* r, GNode other)
{
    assert((*r)&&other);

    ListPush(&other->links, (*r));
    ListPush(&(*r)->prevs, other);

    (*r) = other;

    return 0;
}
int GraphAttach(GNode r, GNode other)
{
    assert(r&&other);

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
    unsigned long num_g = 0;
    BTNode history = InitBT();
    LNode nxt_tmp = tmp->links;

    while (1) {
        /* If current node isn't included in the history ... */
        if (!BTSearch(history, tmp))
            BTInsert(history, tmp);
        else break;

        /* Now find a suitable next node */
        while (nxt_tmp) {
            if (BTSearch(history, nxt_tmp->value))
                nxt_tmp = nxt_tmp->next;
            else {
                tmp = nxt_tmp->value;
                break;
            }
        }
        nxt_tmp = tmp->links;
        ++num_g;
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


    return 0;
}

/* Static method implementations */
int IsRoot(GNode r)
{
    assert(r);
    if (ListIsEmpty(r->prevs)) return 0;
    else return 1;
}
