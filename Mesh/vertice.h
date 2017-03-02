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
    struct _vertice* adjverts; /* Three adjacent vertices */
    int adjnum; /* how many adjacents? */
} vertice;
typedef vertice* VNode;

/* Vertice control node */
typedef struct _vertice_root {
    VNode root_node; /* The root vertice */
    unsigned long num_verts; /* number of vertices assigned in this node */
} vertice_root;
typedef vertice_root* Vertice;

/* Constructors and Destructors */


/* Manipulation methods */


/* Other utils */


#endif /* Include guard */
