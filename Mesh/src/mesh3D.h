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

#include "graph.h"

typedef struct _mesh_3d {
    GNode root_node;

    unsigned long nodes;
    unsigned long vertices;

} mesh_3d;
typedef mesh_3d* Mesh3D;

/* Constructors and Destructors */
Mesh3D Mesh3DInit();
Mesh3D Mesh3DInitGraph(GNode g);

int Mesh3DDestroy(Mesh3D m3d);

/* Manipulation */


#endif /* Include guard */
