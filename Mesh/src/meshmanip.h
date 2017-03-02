/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:34:36-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T14:00:16-06:00
*/
/***************************************

  Mesh Generation Library
  Mesh manipulation methods.

  Header file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#ifndef MESHMANIP_H
#define MESHMANIP_H

#include "mesh.h"

/* Attach to right hand Upper */
int MeshAttachRHU(MNode root, MNode other);
/* Attach to right hand Lower */
int MeshAttachRHL(MNode root, MNode other);
/* Attach to left hand Upper */
int MeshAttachLHU(MNode root, MNode other);
/* Attach to leftt hand Lower */
int MeshAttachLHL(MNode root, MNode other);

/* Attach to bottom left */
int MeshAttachBL(MNode root, MNode other);
/* Attach to bottom right */
int MeshAttachBR(MNode root, MNode other);
/* Attach to upper left */
int MeshAttachUL(MNode root, MNode other);
/* Attach to upper right */
int MeshAttachUR(MNode root, MNode other);



#endif /* Include guard */
