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

    unsigned int i; /* Index for horizontal axis */
    unsigned int j; /* Index for vertical axis */

    struct _mesh_node* rh; /* Right hand side */
    struct _mesh_node* lh; /* Left hand side */
    struct _mesh_node* dn; /* Down side */
    struct _mesh_node* up; /* upper side */

} mesh_node;
typedef mesh_node* MNode;

/* Constructors and Destructors */
MNode NewMesh();
MNode MeshInit();
int MeshDestroy();

/* Simple methods to grow mesh... */
int PushH(MNode* r, MNode n); /* Push from left side */
int AppendH(MNode r, MNode n); /* Attach at the right end */
int PushV(MNode* r, MNode n); /* Push from top side */
int AppendV(MNode r, MNode n); /* Attach at the bottom */

/* Some statistics utils */
unsigned long MeshNodes(MNode m);
void MeshNodesXY(unsigned long* xsize, unsigned long* ysize, MNode m);

/* Traverse Utils */
int MeshTravH(MNode m); /* Horizontal traverse */
int MeshTravV(MNode m); /* Vertical traverse */
int MeshTrav(MNode m); /* X, Y traverse */

/* Find me index */


#endif /* Include guard */
