/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-21T08:37:01-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-21T08:46:11-06:00
*/

/***************************************

  3D Mesh stuff

  Implementation file

  Taylor Shin, Feb. 21 2017

 ***************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mesh3D.h"


/* Constructors and Destructors */
Mesh3D Mesh3DInit()
{
    Mesh3D m3d = (Mesh3D)malloc(sizeof(mesh_3d));
    assert(m3d);

    m3d->nodes = NewGraph();
    m3d->xnodes = 0;
    m3d->ynodes = 0;
    m3d->znodes = 0;

    return m3d;
}

Mesh3D Mesh3DInitGraph(Graph g, unsigned long xn, unsigned long yn, unsigned long zn)
{
    Mesh3D m3d = Mesh3DInit();
    m3d->nodes = g;
    m3d->xnodes = xn;
    m3d->ynodes = yn;
    m3d->znodes = zn;

    return m3d;
}

int Mesh3DDestroy(Mesh3D m3d)
{
    assert(m3d);

    if (m3d->nodes) DeleteGraph(m3d->nodes);
    free(m3d);

    return 0;
}
