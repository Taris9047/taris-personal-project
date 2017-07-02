/**
* @Author: Taylor Shin
* @Date:   2017-03-03T11:09:45-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   Taylor Shin
* @Last modified time: 2017-03-03T13:12:42-06:00
*/
/***************************************

  Graph Pathfinder

  Implementation

  Taylor Shin, Mar. 3 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "path.h"

/* Static functions */
/* Traverse graph with weight */
static int g_trav(GNode r, unsigned long* w, BTree path_db)
{
    assert(r);
    assert(w);

    (*w) += r->index;

    LNode tmpln = r->links;
    GNode tmpgn = (GNode)tmpln->value;

    while (tmpln->next) {
        tmpgn = (GNode)((tmpln->next)->value);
        g_trav(tmpgn, w, path_db);
    }

    (*w) += tmpgn->index;
    BTInsert(path_db, tmpgn, (*w));

    return 0;
}

/* Make weight map */
static int make_weight_map(Graph g, BTree path_db)
{
    assert(g);
    assert(path_db);

    GNode root = g->root_node;
    unsigned long w = root->index; /* Should be most likely zero */

    g_trav(root, &w, path_db);

    return 0;
}



/* Swap nodes withing BTNode recursively */
static int bt_node_sort(BTNode bt)
{
    BTNode left = bt->left;
    BTNode right = bt->right;
    BTNode tmp;

    if (left && right) {
        /* Both side exist, call again for recursion */
        bt_node_sort(bt);
    }
    else if (left && !right) {
        /* Only left side exist: do nothing */
    }
    else if (!left && right) {
        /* Only right side exist for some reason: move it to left side */
        tmp = right;
        bt->left = tmp;
        bt->right = NULL;
    }
    else {
        if (left->key > right->key) {
            tmp = left;
            bt->left = bt->right;
            bt->right = left;
        }
    }

    return 0;
}

/* Constructors and destructors */
GPath PathInit(Graph g)
{
    assert(g);



    GPath p = (GPath)malloc(sizeof(graph_path));
    assert(p);

    p->g = g;
    p->path_db_sort = path_db_sort;

    if (!g->vertices) {
        p->path_db = NULL;
        return p;
    }

    if (make_weight_map(p->g, p->path_db)) {
        free(p);
        return NULL;
    }
    else return p;
}

int PathDestroy(GPath p)
{
    assert(p);

    if (p->g) DeleteGraph(p->g);
    if (p->path_db) BTDelete(p->path_db);
    free(p);

    return 0;
}
int DeletePath(GPath p)
{
    return PathDestroy(p);
}


/* Methods */
/* Find path to given GNode */
List PathFindShort(GPath p, unsigned long key)
{
    assert(p);

    List route = NewList();

    GNode tmpgn;
    p->path_db_sort(p->path_db);

    unsigned long w = 0;
    while (1) {
        tmpgn = GNodeBTSearch(p->path_db, w);
        LPush(route, tmpgn);
        if (tmpgn->index == key) break;
        w++;
    }

    /* Reverse the route list */
    // if (!LReverse(route)) return route;
    // else return NULL;

    /* Nah, just return the reverse list */
    return route;
}

/* Sort path_db */
int path_db_sort(BTree path_db)
{
    assert(path_db);
    return bt_node_sort(path_db->root_node);
}
