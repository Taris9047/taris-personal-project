/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:34:12-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T14:01:23-06:00
*/
/***************************************

  Mesh Generation Library
  Node definitions.

  Implementation file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "mesh.h"

/* Static functions */
static int MeshIsFound(MNode* m, MNode** mlist, int listlen);

/* Constructors and Destructors */
MNode NewMesh()
{
    MNode nm = (MNode)malloc(sizeof(mesh_node));
    if (!nm) return NULL;

    nm->data = NULL;
    nm->rh = NULL;
    nm->lh = NULL;
    nm->dn = NULL;
    nm->up = NULL;
    nm->rd = NULL;
    nm->ul = NULL;

    return nm;
}

/* New mesh with data */
MNode NewMeshData(mesh_data_t data)
{
    MNode nm = NewMesh();
    nm->data = data;
    return nm;
}

/* Alias for NewMesh() */
MNode MeshInit()
{
    return NewMesh();
}

/* Rectangular traverse destructor */
int MeshDestroy(MNode m)
{
    assert(m);

    MNode tmp;
    MNode tmp_h;

    while (1) {
        tmp_h = m->dn;
        while (m->rh) {
            free(m->data);
            tmp = m;
            m = m->rh;
            free(tmp);
        }
        if (tmp_h) m = tmp_h;
        else break;
    }

    return 0;
}

/* Complete traverse destructor */
/* of course, m should be root, the top-left node */
int MeshDestroyAll(MNode m)
{
    assert(m);

    if (m->rh) MeshDestroyAll(m->rh);
    if (m->dn) MeshDestroyAll(m->dn);
    if (m->rd) MeshDestroyAll(m->rd);

    free(m->data);
    free(m);

    return 0;
}

/* Simple methods to grow mesh... */
int PushH(MNode* r, MNode n)
{
    assert(*r);
    assert(n);

    n->rh = (*r);
    (*r)->lh = n;

    if ((*r)->dn) {
        n->rd = (*r)->dn;
        ((*r)->dn)->ul = n;
        if ((*r)->dn->ul->lh) {
            (*r)->dn->ul->lh->up = n;
            n->dn = (*r)->dn->ul->lh;
        }
    }

    (*r) = n;

    return 0;
}

int AppendH(MNode r, MNode n)
{
    assert(r);
    assert(n);

    MNode e = r;
    MeshTravH(&e);
    e->rh = n;
    n->lh = e;

    if (n->lh->up) {
        n->ul = n->lh->up;
        n->lh->up->rd = n;
    }

    return 0;
}

int PushV(MNode* r, MNode n)
{
    assert(*r);
    assert(n);

    n->dn = (*r);
    (*r)->up = n;

    if ((*r)->rh) {
        n->rd = (*r)->rh;
        (*r)->rh->ul = n;
        if ((*r)->rh->up) {
            n->rh = (*r)->rh->up;
            (*r)->rh->up->lh = n;
        }
    }

    (*r) = n;

    return 0;
}

int AppendV(MNode r, MNode n)
{
    assert(r);
    assert(n);

    MNode e = r;
    MeshTravV(&e);
    e->dn = n;
    n->up = e;

    if (n->up->lh) {
        n->ul = n->up->lh;
        n->up->lh->rd = n;
    }

    return 0;
}

/* Some statistics utils */
unsigned long MeshNodes(MNode m)
{
    assert(m);

    unsigned long size = 0;
    MNode r = m;
    MeshTravAll(&r, &size);

    return 0;
}
/* Only use if you are sure that the mesh is rectangular!! */
void MeshNodesXY(MNode m, unsigned long* xsize, unsigned long* ysize)
{
    assert(m);
    assert(xsize); assert(ysize);

    unsigned long xs = 0;
    unsigned long ys = 0;

    MNode bck = m;
    while (m) {
        m = m->rh;
        ++xs;
    }
    m = bck;
    while (m) {
        m = m->dn;
        ++ys;
    }

    (*xsize) = xs;
    (*ysize) = ys;

    return;
}

/* Traverse Utils */
/* Warning!! they change given pointer!! */
int MeshTravH(MNode* m)
{
    assert(*m);

    unsigned long i = 0;
    while((*m)->rh) {
        (*m) = (*m)->rh; ++i;
    }

    return i;
}

int MeshTravV(MNode* m)
{
    assert(*m);

    unsigned long i = 0;
    while((*m)->dn) {
        (*m) = (*m)->dn; ++i;
    }

    return i;
}

/* Rectangular traverse */
int MeshTrav(MNode* m, unsigned long* i, unsigned long* j)
{
    assert(*m);
    assert(i);
    assert(j);

    (*i) = 0;
    (*j) = 0;

    MNode tmp_v = (*m);

    while(tmp_v) {
        while((*m)->rh) {
            (*m) = (*m)->rh;
            ++(*i);
        }
        (*m) = tmp_v->dn;
        tmp_v = tmp_v->dn;
        (*i) = 0;
        ++(*j);
    }

    return 0;
}

/* Complete Traverse */
int MeshTravAll(MNode* m, unsigned long* n_tot)
{
    assert(*m);

    MNode tmp = (*m);
    (*n_tot) = 1; /* self */
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */
    ListPush(&ml, tmp); /* Push in first node */

    /* The search algorithm...
       1st rh
       2nd rd
       3rd dn
       4th lh
       5th up
    */
    while (1) {
        if (tmp->rh && !ListFind(ml, tmp->rh)) {
            ListPush(&ml, tmp->rh);
            tmp = tmp->rh;
            ++(*n_tot);
            continue;
        }

        if (tmp->rd && !ListFind(ml, tmp->rd)) {
            ListPush(&ml, tmp->rd);
            tmp = tmp->rd;
            ++(*n_tot);
            continue;
        }

        if (tmp->dn && !ListFind(ml, tmp->dn)) {
            ListPush(&ml, tmp->dn);
            tmp = tmp->dn;
            ++(*n_tot);
            continue;
        }

        if (tmp->lh && !ListFind(ml, tmp->lh)) {
            ListPush(&ml, tmp->lh);
            tmp = tmp->lh;
            ++(*n_tot);
            continue;
        }

        if (tmp->up && !ListFind(ml, tmp->up)) {
            ListPush(&ml, tmp->up);
            tmp = tmp->up;
            ++(*n_tot);
            continue;
        }

        (*m) = tmp;
        break;
    }

    ListDestroy(ml);
    return 0;
}

/* Find by data */
MNode Find(mesh_data_t stuff)
{
    return NULL;
}

/* find given mesh from mesh list */
static int MeshIsFound(MNode* m, MNode** mlist, int listlen)
{
    assert(m);
    int i;

    if (!mlist) return 0;
    for (i=0; i<listlen; ++i) if (m == mlist[i]) return 1;

    return 0;
}
