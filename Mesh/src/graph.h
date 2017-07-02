/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T08:37:01-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T08:38:17-06:00
*/

/***************************************

  Graph Data Structure for Mesh

  Header file

  Taylor Shin, Feb. 21 2017

 ***************************************/

#ifndef MESH_GRAPH_H
#define MESH_GRAPH_H

#include "list.h"
#define GNodeLAt(lst, num) ((GNode)LAt(lst, num))
#define ULONGLAt(lst, num) (*(unsigned long*)LAt(lst, num))

#define MATRIX_D_T int
#include "matrix.h"

//#define SPARSE_MODE

/* data type */
typedef void* graph_data_t;

/* Graph node */
typedef struct _graph_node {
    graph_data_t data; /* The data */

    /* Linked lists
        Why not binary tree?
        We suspect each node would have
        only up to 3 - 6 links at most.
        We'll implement binary tree if
        needed...
    */
    LNode links; /* Other nodes linked from this one */
    LNode prevs; /* Other nodes linked To this one */

    unsigned long edges; /* Number of outgoing edges from this node */
    unsigned long index; /* Binary tree search key */

} graph_node;
typedef graph_node* GNode;

/* Graph root (with regular matrix) */
typedef struct _graph_root {
    GNode root_node;
    unsigned long vertices;
    unsigned long edges;
    Matrix adjMatrix;
} graph_root;
typedef graph_root* Graph;

/* Graph root (with sparse matrix) */
typedef struct _graph_root_s {
    GNode root_node;
    unsigned long vertices;
    unsigned long edges;
    SMatrix adjMatrix;
} graph_root_s;
typedef graph_root_s* SGraph;



/* Constructors and Destructors */
/* Control node */
/* Regular matrix */
Graph NewGraph();
int DeleteGraph(Graph g);
/* Sparse matrix */
SGraph NewSGraph();
int DeleteSGraph(SGraph g);

/* for nodes */
GNode init_graph_node();
GNode init_graph_node_data(graph_data_t data);
GNode graph_make_node(graph_data_t data);

int graph_node_destroy(GNode r);

/* Manipulation methods */
int GraphAttach(Graph g, GNode n, List keys);
/* for nodes */
int graph_node_attach(GNode n, GNode* adjs, unsigned long n_adjs);
int graph_node_find(Graph g, GNode* r, unsigned long key);

// int GraphPush(GNode* r, GNode other);
// int GraphAttach(GNode r, GNode other);

/* Utilities */
//int GraphTraverse(GNode* r, unsigned long* n_tot);
//int GraphFind(GNode* r, GNode* found, graph_data_t data);


#endif /* Include guard */
