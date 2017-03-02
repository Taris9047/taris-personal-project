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

    unsigned long index; /* Binary tree search key */

} graph_node;
typedef graph_node* GNode;

/* Graph root (with regular matrix) */
typedef struct _graph_root {
    GNode root;
    unsigned long vertices;
    unsigned long edges;
    Matrix adjMatrix;
} graph_root;
typedef graph_root* Graph;

/* Graph root (with sparse matrix) */
typedef struct _graph_root_s {
    GNode root;
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
GNode InitGraph();
GNode InitGraphData(graph_data_t data);
GNode GraphMakeNode(graph_data_t data);

int GraphDestroy(GNode r);

/* Manipulatin methods */
int GraphPush(GNode* r, GNode other);
int GraphAttach(GNode r, GNode other);

/* Utilities */
int GraphTraverse(GNode* r, unsigned long* n_tot);
int GraphFind(GNode* r, GNode* found, graph_data_t data);

/* Add/Sub/Multiply/Divide matrix data */
void MAddSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
void MSubSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
void MMulSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
void MDivSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
void MRemSc(matrix_data_t* r, matrix_data_t a, matrix_data_t b);
void MSetZero();

#endif /* Include guard */
