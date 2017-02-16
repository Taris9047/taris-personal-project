/***************************************

  Mesh Generation Library
  Node definitions.

  Implementation file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"

/* Constructors and Destructors */
MNode NewMesh()
{
    MNode nm = (MNode)malloc(sizeof(mesh_node));
    if (!nm) return NULL;

    nm->i = 0;
    nm->j = 0;
    nm->data = NULL;
    nm->rh = NULL;
    nm->lh = NULL;
    nm->dn = NULL;
    nm->up = NULL;

    return nm;
}

MNode MeshInit()
{
    return NewMesh();
}

int MeshDestroy()
{
    return 0;
}
