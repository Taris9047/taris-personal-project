/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:34:07-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T14:00:26-06:00
*/
/***************************************

  Mesh Generation Library
  Mesh generation algorithm

  Header file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#ifndef MESH_GEN_H
#define MESH_GEN_H

#include "mesh.h"
#include "meshmanip.h"

/* Make rectangular mesh */
MNode MeshMakeRect(unsigned long rows, unsigned long cols);
/* Make rectangular mesh with dataset */
MNode MeshMakeRectData(unsigned long rows, unsigned long cols, mesh_data_t** data);
/* Set data to current mesh */
int MeshSetData(MNode r, mesh_data_t** data, unsigned long rows, unsigned long cols);

/* Make mesh_data_t 2D array */
void MeshDMalloc(mesh_data_t*** mesh_d_2D, unsigned long rows, unsigned long cols);
/* Free mesh_data_t 2D array */
void MeshDFree(mesh_data_t** mesh_d_2D, unsigned long rows, unsigned long cols);


#endif /* Include guard */
