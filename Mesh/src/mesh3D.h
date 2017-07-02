/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T08:37:01-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T08:46:19-06:00
*/

/***************************************

  3D Mesh Stuff

  Header file

  Taylor Shin, Feb. 21 2017

 ***************************************/

#ifndef MESH3D_H
#define MESH3D_H

/* TODO: Maybe finish this later ... let's finish up 2D mesh first */

#include "graph.h"

typedef struct _mesh_3d {
    Graph nodes; /* Each node is graph */
    /*
        Each graph has...
        1. multiple adjacent nodes (it's 3D)
        2. information on how to navigate.
        3. and index.
    */

    unsigned long xnodes;
    unsigned long ynodes;
    unsigned long znodes;

} mesh_3d;
typedef mesh_3d* Mesh3D;

/* Constructors and Destructors */
Mesh3D Mesh3DInit();
Mesh3D Mesh3DInitGraph(Graph g, unsigned long xn, unsigned long yn, unsigned long zn);

int Mesh3DDestroy(Mesh3D m3d);

/* Manipulation */


#endif /* Include guard */
