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

/* Make rectangular mesh */
/* Note i and j are actual element numbers, not index */
MNode MeshMakeRect(unsigned long i, unsigned long j)
{
    MNode rect_mesh = NewMesh();
    MNode tmp_new;

    unsigned long hi, vi;
    for (hi=1; hi<i; ++hi) {
        tmp_new = NewMesh();
        PushH(&rect_mesh, tmp_new);
    }
    for (vi=1; vi<j; ++vi) {
        tmp_new = NewMesh();
        PushV(&rect_mesh, tmp_new);
    }

    return 0;
}

/* Make rectangular mesh with dataset */
/* Note i and j are actual element numbers, not index */
MNode MeshMakeRectData(unsigned long i, unsigned long j, mesh_data_t** data)
{
    MNode rect_mesh = MeshMakeRect(i, j);

    if (!MeshSetData(rect_mesh, data, i, j)) return rect_mesh;
    else return NULL;
}

/* Set data to current mesh */
/* Note i and j are actual element numbers, not index */
int MeshSetData(MNode r, mesh_data_t** data, unsigned long i, unsigned long j)
{
    unsigned long ih, iv;
    MNode tmp;
    MNode tmp_h;

    tmp = r;
    for (ih=0; ih<i; ++ih) {
        tmp_h = tmp;
        for (iv=0; iv<j; ++iv) {
            tmp->data = data[ih][iv];
            tmp = tmp->dn;
        }
        tmp = tmp_h->rh;
    }

    return 0;
}
