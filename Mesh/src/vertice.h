/**
* @Author: Taylor Shin
* @Date:   2017-03-01T16:40:00-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   Taylor Shin
* @Last modified time: 2017-03-01T16:40:38-06:00
*/
/***************************************

  Vertice Generation Library

  Header file

  Taylor Shin, Mar. 01 2017

 ***************************************/

#ifndef MESH_VERTICE_H
#define MESH_VERTICE_H

/* Uses the mesh datastructure */
#include "meshmanip.h"

/* Vertice node definition */
typedef struct _vertice {
    MNode points[3]; /* Three points - mesh nodes */
    struct _vertice* adjverts[3]; /* Three adjacent vertices */
    /*
        adjverts[0] -> line between point[0] and point[1]
        adjverts[1] -> line between point[1] and point[2]
        adjverts[2] -> line between point[2] and point[0]
    */
    int adjnum; /* how many adjacents? */
    int is_root; /* is this a root vertice? */
} vertice;
typedef vertice* VNode;

/* Vertice control node */
typedef struct _vertice_root {
    VNode root_node; /* The root vertice */
    unsigned long num_verts; /* number of vertices assigned in this node */
} vertice_root;
typedef vertice_root* Vertice;

/* Constructors and Destructors */
/* Root node */
Vertice NewVertice();
int DeleteVertice(Vertice v);

/* Node manipulation */
VNode InitVNode();
VNode MakeVNode(MNode a, MNode b, MNode c);
int DestroyVNode(VNode v);

/* Manipulation methods */
/* Control nodes */
/* Nodes */
int AttachVNode(VNode r, VNode o);
int DetachVNode(VNode r, VNode o);
int AttachPoint(VNode r, MNode m, VNode* n, int face);


/* Other utils */


#endif /* Include guard */
