/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:34:36-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T14:00:39-06:00
*/
/***************************************

  Mesh Generation Library
  Mesh manipulation methods.

  Implementation file

  Taylor Shin, Feb. 16 2017

 ***************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "meshmanip.h"

/* Some extern functions */
/* ID Generator */
//extern inline unsigned long MeshIDGen(unsigned long a, unsigned long b);

/* Get size macro */
#define GET_SIZE \
    unsigned long root_rows, root_cols;\
    unsigned long other_rows, other_cols;\
    MeshNodesXY(root, &root_rows, &root_cols);\
    MeshNodesXY(other, &other_rows, &other_cols);\


/* Static functions (make sure root is larger than other) */
/* Attach Right side upper */
static int AttachRU(MNode root, MNode other);
/* Attach Right side lower */
static int AttachRL(MNode root, MNode other);
/* Attach Lower right corner */
static int AttachLR(MNode root, MNode other);
/* Attach Lower left fornter */
static int AttachLL(MNode root, MNode other);
/* Attach Right side offset */
static int AttachR(MNode root, MNode other, unsigned long offset);
/* attach Lower side offset */
static int AttachL(MNode root, MNode other, unsigned long offset);
/* Advance horizontally (towards rightside) */
static int AdvH(MNode* n, unsigned long off);
/* Advance vertically (towards downside) */
static int AdvV(MNode* n, unsigned long off);
/* Update mesh IDs */
static int UpdateID(MNode m, unsigned long st_r, unsigned long st_c, unsigned long nr, unsigned long nc);

/* TODO: Add ID update code. */

/* Attach to right hand Upper */
int MeshAttachRHU(MNode root, MNode other)
{
    GET_SIZE

    if (root_rows < other_rows) MeshSwap(&root, &other);

    AttachRU(root, other);

    return 0;
}
/* Attach to right hand Lower */
int MeshAttachRHL(MNode root, MNode other)
{
    GET_SIZE

    if (root_rows < other_rows) MeshSwap(&root, &other);

    AttachLR(root, other);

    return 0;
}
/* Attach to left hand Upper */
int MeshAttachLHU(MNode root, MNode other)
{
    GET_SIZE

    if (root_rows > other_rows) MeshSwap(&root, &other);

    AttachRU(other, root);

    return 0;
}
/* Attach to leftt hand Lower */
int MeshAttachLHL(MNode root, MNode other)
{
    GET_SIZE

    if (root_rows > other_rows) MeshSwap(&root, &other);

    AttachRL(other, root);

    return 0;
}

/* Attach to bottom left */
int MeshAttachBL(MNode root, MNode other)
{
    GET_SIZE

    if (root_cols < other_cols) MeshSwap(&root, &other);

    AttachLL(root, other);

    return 0;
}
/* Attach to bottom right */
int MeshAttachBR(MNode root, MNode other)
{
    GET_SIZE

    if (root_cols < other_cols) MeshSwap(&root, &other);

    AttachLR(root, other);

    return 0;
}
/* Attach to upper left */
int MeshAttachUL(MNode root, MNode other)
{
    GET_SIZE

    if (root_cols > other_cols) MeshSwap(&root, &other);

    AttachLL(other, root);

    return 0;
}
/* Attach to upper right */
int MeshAttachUR(MNode root, MNode other)
{
    GET_SIZE

    if (root_cols > other_cols) MeshSwap(&root, &other);

    AttachLR(other, root);

    return 0;
}





/* Static attachment functions */
/* Make sure other is shorter at the contact side */
/* Attach Right side upper */
static int AttachRU(MNode root, MNode other)
{
    assert(root);
    assert(other);

    AttachR(root, other, 0);

    return 0;
}
/* Attach Right side lower */
static int AttachRL(MNode root, MNode other)
{
    assert(root);
    assert(other);

    MNode o_tmp = other;
    MNode r_tmp = root;
    unsigned long o_rows = MeshTravV(&o_tmp);
    unsigned long r_rows = MeshTravV(&r_tmp);

    AttachR(root, other, r_rows-o_rows);

    return 0;
}
/* Attach Lower right corner */
static int AttachLR(MNode root, MNode other)
{
    assert(root);
    assert(other);

    MNode o_tmp = other;
    MNode r_tmp = root;
    unsigned long o_cols = MeshTravH(&o_tmp);
    unsigned long r_cols = MeshTravH(&r_tmp);

    AttachL(root, other, r_cols-o_cols);

    return 0;
}
/* Attach Lower left fornter */
static int AttachLL(MNode root, MNode other)
{
    assert(root);
    assert(other);

    AttachL(root, other, 0);

    return 0;
}
/* Attach Right side offset */
static int AttachR(MNode root, MNode other, unsigned long offset)
{
    assert(root);
    assert(other);

    MNode r_tmp = root;
    MNode o_tmp = other;

    unsigned long i;
    unsigned long root_rows = MeshTravV(&r_tmp);
    unsigned long oth_rows = MeshTravV(&o_tmp);

    /* Error, wrong offset */
    if (oth_rows + offset > root_rows) return -1;

    /* return the pointers to original node */
    r_tmp = root; o_tmp = other;

    /* traverse root pointer to right edge */
    MeshTravH(&r_tmp);

    /* advance pointers by offset */
    AdvV(&r_tmp, offset);

    /* Then attach them */
    for (i=0; i<oth_rows-offset; ++i) {
        /* attach current two nodes */
        r_tmp->rh = o_tmp;
        o_tmp->lh = r_tmp;

        /* if other node has lower node,
           update rd as well */
        if (o_tmp->dn) {
            r_tmp->rd = o_tmp->dn;
            o_tmp->dn->ul = r_tmp;
        }

        /* Then advance the pointers */
        r_tmp = r_tmp->dn;
        o_tmp = o_tmp->dn;
    }

    return 0;
}
/* attach Lower side offset */
static int AttachL(MNode root, MNode other, unsigned long offset)
{
    assert(root);
    assert(other);

    MNode r_tmp = root;
    MNode o_tmp = other;

    unsigned long i;
    unsigned long root_cols = MeshTravH(&r_tmp);
    unsigned long oth_cols = MeshTravH(&o_tmp);

    /* Error, wrong offset */
    if (oth_cols + offset > root_cols) return -1;

    /* return the pointers to original node */
    r_tmp = root; o_tmp = other;

    /* traverse root pointer to right edge */
    MeshTravV(&r_tmp);

    /* advance pointers by offset */
    AdvH(&r_tmp, offset);

    /* Then attach them */
    for (i=0; i<oth_cols-offset; ++i) {
        r_tmp->dn = o_tmp;
        o_tmp->up = r_tmp;

        if (r_tmp->lh) {
            o_tmp->ul = r_tmp->lh;
            r_tmp->lh->rd = o_tmp;
        }

        r_tmp = r_tmp->rh;
        o_tmp = o_tmp->rh;
    }

    return 0;
}

/* Advance horizontally (towards rightside) */
static int AdvH(MNode* n, unsigned long off)
{
    assert(*n);
    if (!off) return 0;

    unsigned long i;
    for (i=0; i<off; ++i) (*n) = (*n)->rh;
    return 0;
}

/* Advance vertically (towards downside) */
static int AdvV(MNode* n, unsigned long off)
{
    assert(*n);
    if (!off) return 0;

    unsigned long i;
    for (i=0; i<off; ++i) (*n) = (*n)->dn;
    return 0;
}

/* Update mesh IDs */
static int UpdateID(MNode m, unsigned long st_r, unsigned long st_c, unsigned long nr, unsigned long nc)
{
    assert(m);

    MNode tmp_h, tmp;
    unsigned long i, j;

    tmp = m;

    for (i=0; i<nr; ++i) {
        tmp_h = tmp->rh;
        for (j=0; j<nc; ++j) {
            tmp->id = MeshIDGen(st_r+i, st_c+j);
            tmp = tmp->dn;
        }
        if (tmp_h) tmp = tmp_h;
        else break;
    }

    return 0;
}
