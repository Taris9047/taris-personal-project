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
//static int MeshIsFound(MNode* m, MNode** mlist, int listlen);

/* Constructors and Destructors */
MNode NewMesh()
{
    MNode nm = (MNode)malloc(sizeof(mesh_node));
    //if (!nm) return NULL;
    assert(nm);

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
    if (data) nm->data = data;
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
    MNode tmp;
    MNode tmp_h;

    /* empty node, nothing to destroy */
    if (!m) return 0;

    while (1) {
        tmp_h = m->dn;
        while (m) {
            //if (m->data) free(m->data);
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

    //free(m->data);
    free(m);

    return 0;
}

/* Simple methods to grow mesh... */
int PushH(MNode* r, MNode n)
{
    assert(*r);
    assert(n);

    MNode tmp = (*r);

    n->rh = tmp;
    tmp->lh = n;

    if (tmp->dn) {
        n->rd = tmp->dn;
        tmp->dn->ul = n;
        if (tmp->dn->lh) {
            n->dn = tmp->dn->lh;
            tmp->dn->lh->up = n;
        }
    }
    else if (tmp->up) {
        if (tmp->up->lh) {
            n->up = tmp->up->lh;
            tmp->up->lh->dn = n;
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

    /* Update diagonal axis */
    if (n->lh->up) {
        n->ul = n->lh->up;
        n->lh->up->rd = n;

        /* Update perpendicular axis */
        if (n->lh->up->rh) {
            n->up = n->lh->up->rh;
            n->lh->up->rh->dn = n;
        }
    }

    return 0;
}

int PushV(MNode* r, MNode n)
{
    assert(*r);
    assert(n);

    MNode tmp = (*r);

    n->dn = tmp;
    tmp->up = n;

    if (tmp->lh) {
        if (tmp->lh->up) {
            tmp->ul = tmp->lh->up;
            tmp->lh->up->rd = tmp;
            n->lh = tmp->lh->up;
            tmp->lh->up->rh = n;
        }
    }
    else if (tmp->rh) {
        n->rd = tmp->rh;
        tmp->ul = n;
        if (tmp->rh->up) {
            n->rh = tmp->rh->up;
            tmp->rh->up->lh = n;
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

    /* Update diagonal axis */
    if (n->up->lh) {
        n->ul = n->up->lh;
        n->up->lh->rd = n;

        /* Update perpendicular axis */
        if (n->up->lh->dn) {
            n->lh = n->up->lh->dn;
            n->up->lh->dn->rh = n;
        }
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
unsigned long MeshTravH(MNode* m)
{
    assert(*m);

    unsigned long i = 0;
    while((*m)->rh) {
        (*m) = (*m)->rh; ++i;
    }

    return i;
}

unsigned long MeshTravV(MNode* m)
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
MNode MeshFind(MNode* m, mesh_data_t stuff)
{
    assert(*m);

    MNode tmp = (*m);
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
        if (tmp->data != stuff) {
            if (tmp->rh && !ListFind(ml, tmp->rh)) {
                ListPush(&ml, tmp->rh);
                tmp = tmp->rh;
                continue;
            }

            if (tmp->rd && !ListFind(ml, tmp->rd)) {
                ListPush(&ml, tmp->rd);
                tmp = tmp->rd;
                continue;
            }

            if (tmp->dn && !ListFind(ml, tmp->dn)) {
                ListPush(&ml, tmp->dn);
                tmp = tmp->dn;
                continue;
            }

            if (tmp->lh && !ListFind(ml, tmp->lh)) {
                ListPush(&ml, tmp->lh);
                tmp = tmp->lh;
                continue;
            }

            if (tmp->up && !ListFind(ml, tmp->up)) {
                ListPush(&ml, tmp->up);
                tmp = tmp->up;
                continue;
            }
        }
        else {
            (*m) = tmp;
            break;
        }
    }

    ListDestroy(ml);
    return 0;
}

/* Find Root */
int MeshFindRoot(MNode *m)
{
    assert(*m);

    MNode tmp = (*m);

    /* If it's root, just skip all the cruds */
    if (MeshIsRoot(tmp)) return 0;

    /* Traverse back x */
    while (tmp->lh || tmp->ul || tmp->up) {
        if (tmp->lh) {
            tmp = tmp->lh;
            continue;
        }
        if (tmp->ul) {
            tmp = tmp->ul;
            continue;
        }
        if (tmp->up) {
            tmp = tmp->up;
            continue;
        }
    }

    (*m) = tmp;

    return 0;
}

/* Swap Meshes */
int MeshSwap(MNode* a, MNode* b)
{
    assert(*a);
    assert(*b);
    /* If a and b are the same... just end this crap */
    if ((*a) == (*b)) return 0;

    MNode tmp;
    tmp = (*a);
    (*a) = (*b);
    (*b) = tmp;

    return 0;
}

/* Copy meshes */
int MeshCpy(MNode* target, MNode a)
{
    assert(a);

    unsigned long a_rows, a_cols;
    unsigned long i, j;
    mesh_data_t tmp_data;
    MNode tmp_head;
    MNode tmp_node;
    MNode tmp_a;

    /* make sure a is root */
    if (!MeshIsRoot(a)) MeshFindRoot(&a);

    /* Set up the first node */
    tmp_data = a->data;
    tmp_head = NewMeshData(tmp_data);

    /* figure out size of a */
    MeshNodesXY(a, &a_rows, &a_cols);

    /* Ok, let's generate another mesh */
    /* Generating the first row */
    tmp_a = a;
    /* Set tmp to lower right corner */
    MeshTravH(&tmp_a);
    MeshTravV(&tmp_a);
    /* Generate lowest line */
    for (i=1; i<a_rows; ++i) {
        tmp_data = tmp_a->lh->data;
        tmp_node = NewMeshData(tmp_data);
        PushH(target, tmp_node);
        tmp_a = tmp_a->lh;
    }

    /* Extending from first row */
    /* Returning tmp_data to the rightmost one */
    MeshTravH(&tmp_head);
    /* Also a */
    MeshTravH(&tmp_a);
    tmp_a = tmp_a->up;
    for (j=1; j<a_cols; ++j) {
        tmp_node = NewMeshData(tmp_a->data);
        PushV(&tmp_head, tmp_node);
        for (i=1; i<a_rows; ++i) {
            tmp_data = tmp_a->lh->data;
            tmp_node = NewMeshData(tmp_data);
            PushH(&tmp_head, tmp_node);
            tmp_a = tmp_a->lh;
        }
        if (j<a_cols) {
            MeshTravH(&tmp_head);
            MeshTravH(&tmp_a);
            tmp_a = tmp_a->up;
        }
    }

    (*target) = tmp_head;

    return 0;
}

/* Is root? (top left corner) */
int MeshIsRoot(MNode a)
{
    assert(a);
    if (!a->lh && !a->up && !a->ul) return 1;
    else return 0;
}

/* find given mesh from mesh list */
// static int MeshIsFound(MNode* m, MNode** mlist, int listlen)
// {
//     assert(m);
//     int i;
//
//     if (!mlist) return 0;
//     for (i=0; i<listlen; ++i) if (m == mlist[i]) return 1;
//
//     return 0;
// }
