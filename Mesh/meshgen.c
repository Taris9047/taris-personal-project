/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:34:07-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T14:00:54-06:00
*/
/***************************************

  Mesh Generation Library
  Mesh generation algorithm

  Implementation file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "meshgen.h"

/* Some extern functions */
/* ID Generator */
extern inline unsigned long MeshIDGen(unsigned long a, unsigned long b);

/* Some static functions */

/* Make rectangular mesh */
/* Note i and j are actual element numbers, not index */
MNode MeshMakeRect(unsigned long rows, unsigned long cols)
{
    MNode rect_mesh = NewMesh();
    MNode tmp_new;
    MNode tmp_h;
    MNode tmp_v;

    unsigned long hi, vi;

    rect_mesh->id = MeshIDGen(0, 0);
    for (hi=1; hi<rows; ++hi) {
        tmp_new = NewMesh();
        tmp_new->id = MeshIDGen(hi, 0);
        PushH(&rect_mesh, tmp_new);
    }
    tmp_v = tmp_h = rect_mesh;
    while (1) {
        for (vi=1; vi<cols; ++vi) {
            tmp_new = NewMesh();
            tmp_new->id = MeshIDGen(hi, vi);
            PushV(&tmp_v, tmp_new);
        }
        tmp_v = tmp_h;
        if (tmp_v->rh) {
            tmp_v = tmp_v->rh;
            tmp_h = tmp_v;
        }
        else break;
    }

    MeshFindRoot(&tmp_v);

    rect_mesh = tmp_v;

    return rect_mesh;
}

/* Make rectangular mesh with dataset */
/* Note i and j are actual element numbers, not index */
MNode MeshMakeRectData(unsigned long rows, unsigned long cols, mesh_data_t** data)
{
    MNode rect_mesh = MeshMakeRect(rows, cols);

    if (!MeshSetData(rect_mesh, data, rows, cols))
        return rect_mesh;
    else
        return NULL;
}

/* Set data to current mesh */
/* Note i and j are actual element numbers, not index */
int MeshSetData(MNode r, mesh_data_t** data, unsigned long rows, unsigned long cols)
{
    unsigned long ih, iv;
    MNode tmp;
    MNode tmp_h;

    tmp = r;
    for (ih=0; ih<rows; ++ih) {
        tmp_h = tmp;
        for (iv=0; iv<cols; ++iv) {
            tmp->data = data[ih][iv];
            tmp = tmp->dn;
        }
        tmp = tmp_h->rh;
    }

    return 0;
}

/* Make mesh_data_t 2D array */
void MeshDMalloc(mesh_data_t*** mesh_d_2D, unsigned long rows, unsigned long cols)
{
    /* It won't happen but sometimes people gets nuts. */
    if (rows == 0 || cols == 0) {
        (*mesh_d_2D) = NULL;
        return;
    }

    unsigned long i;

    mesh_data_t** t_md_2D;

    t_md_2D = (mesh_data_t**)malloc(sizeof(mesh_data_t*)*rows);
    assert(t_md_2D);
    for (i=0; i<rows; ++i) {
        t_md_2D[i] = (mesh_data_t*)malloc(sizeof(mesh_data_t)*cols);
        assert(t_md_2D[i]);
    }

    (*mesh_d_2D) = t_md_2D;

    return;
}

/* Free mesh_data_t 2D array */
void MeshDFree(mesh_data_t** mesh_d_2D, unsigned long rows, unsigned long cols)
{
    assert(mesh_d_2D);

    unsigned long i, j;

    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            free(mesh_d_2D[i][j]);
        }
    }
    for (i=0; i<rows; ++i) free(mesh_d_2D[i]);
    free(mesh_d_2D);

    return;
}
