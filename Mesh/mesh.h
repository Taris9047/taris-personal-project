/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:34:12-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T14:06:03-06:00
*/
/***************************************

  Mesh Generation Library
  Node definitions.

  Header file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#ifndef MESH_MAIN_H
#define MESH_MAIN_H

/* A mesh point in a semiconductor material */
/* Numerical data will be handled by general pointers */
typedef void* mesh_data_t;

/* A mesh node */
/* At this moment, the mesh generation is assumed with 2D
   device. We'll extend it to a 3D mesh with cascaded 2D meshes.
   */
typedef struct _mesh_node {
    mesh_data_t data; /* Make a custom struct to store a data set for each node */

    struct _mesh_node* rh; /* Right hand side */
    struct _mesh_node* lh; /* Left hand side */
    struct _mesh_node* dn; /* Down side */
    struct _mesh_node* up; /* upper side */
    struct _mesh_node* rd; /* right-down side */
    struct _mesh_node* ul; /* upper-left side */

} mesh_node;
typedef mesh_node* MNode;

/* Constructors and Destructors */
MNode NewMesh();
MNode NewMeshData(mesh_data_t data);
MNode MeshInit();
int MeshDestroy(MNode m);
int MeshDestroyAll(MNode m);

/* Simple methods to grow mesh... */
int PushH(MNode* r, MNode n); /* Push from left side */
int AppendH(MNode r, MNode n); /* Attach at the right end */
int PushV(MNode* r, MNode n); /* Push from top side */
int AppendV(MNode r, MNode n); /* Attach at the bottom */

/* Some statistics utils */
unsigned long MeshNodes(MNode m);
void MeshNodesXY(MNode m, unsigned long* xsize, unsigned long* ysize);

/* Traverse Utils */
unsigned long MeshTravH(MNode* m); /* Horizontal traverse */
unsigned long MeshTravV(MNode* m); /* Vertical traverse */
int MeshTrav(MNode* m, unsigned long* i, unsigned long* j); /* X, Y traverse */
int MeshTravAll(MNode* m, unsigned long* n_tot); /* Complete traverse */

/* Find stuff */
MNode MeshFind(MNode* m, mesh_data_t stuff);
/* Find Root (Assuning rectangular) */
int MeshFindRoot(MNode* m);
/* Swap meshes */
int MeshSwap(MNode* a, MNode* b);
/* Copy meshes (only works with rectangular mesh at this moment) */
int MeshCpy(MNode* target, MNode a);
/* Is root? (top left corner) */
int MeshIsRoot(MNode a);

#endif /* Include guard */
