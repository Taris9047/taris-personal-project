/**
* @Author: Taylor Shin
* @Date:   2017-03-01T16:40:00-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   Taylor Shin
* @Last modified time: 2017-03-01T16:41:17-06:00
*/
/***************************************

  Vertice Generation Library

  Implementation file

  Taylor Shin, Mar. 01 2017

 ***************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdlib.h>

#include "vertice.h"

/* Static functions - mainly manipulating the nodes */
/* Match nodes - match two 3 element nodes */
static int MatchNodes(void* an[3], void* bn[3], int mA[3], int mB[3])
{
    assert(an); assert(bn);

    /*
       each an, bn must be distinct element array
       But, we're not gonna check them now...
    */

    int i, j;
    int matches = 0;

    for (i=0; i<3; ++i) {
        for (j=0; j<3; ++j) {
            if (an[i] == bn[j]) {
                mA[i] = j;
                mB[j] = i;
                matches++;
            }
            else {
                mA[i] = -1;
                mB[j] = -1;
            }
        }
    }

    return matches;
}

/* match line */
/* given isolated node, returns the facing line number */
static inline int MLTbl(const int nn)
{
    switch(nn) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 0;
    }
    return 0;
}



/* Constructors and destructors */
/* Root node */
Vertice NewVertice()
{
    Vertice v = (Vertice)malloc(sizeof(vertice_root));
    assert(v);

    v->root_node = NULL;
    v->num_verts = 0;

    return v;
}
int DeleteVertice(Vertice v)
{
    assert(v);

    if (v->root_node) DestroyVNode(v->root_node);
    free(v);

    return 0;
}

/* Node manipulation */
VNode InitVNode()
{
    VNode vn = (VNode)malloc(sizeof(vertice_root));
    assert(vn);

    int i;
    for (i=0; i<3; ++i) vn->points[i] = NULL;
    for (i=0; i<3; ++i) vn->adjverts[i] = NULL;

    vn->adjnum = 0;
    vn->is_root = 0;

    return vn;
}
VNode MakeVNode(MNode a, MNode b, MNode c)
{
    assert(a); assert(b); assert(c);

    VNode vn = InitVNode();
    vn->points[0] = a;
    vn->points[1] = b;
    vn->points[2] = c;

    return vn;
}
int DestroyVNode(VNode v)
{
    assert(v);

    int i;
    for (i=0; i<3; ++i) DestroyVNode(v->adjverts[i]);
    free(v);

    return 0;
}

/* Manipulation methods */
/* Control nodes */
/* Nodes */
int AttachVNode(VNode r, VNode o)
{
    assert(r); assert(o);
    assert(r!=o);

    MNode rp[3];
    MNode op[3];
    int rmatch[3];
    int omatch[3];

    int match_num;

    int i;
    for (i=0; i<3; ++i) rp[i] = r->points[i];
    for (i=0; i<3; ++i) op[i] = o->points[i];

    match_num = MatchNodes((void**)rp, (void**)op, rmatch, omatch);

    if (match_num == 1) {
        /* Only one overlapping point: dangling triangles... */
        /* Do nothing */
        return match_num;
    }
    else if (match_num == 0) {
        /* Totally separated node. Unable to attach */
        return -1;
    }

    /* Ok, at this point, let's assume we have 2 matches */
    /* find out where the match happened */
    for(i=0; i<3; ++i) if( rmatch[i] == -1 ) break;
    /* Then, actually attach the o vertice */
    r->adjverts[MLTbl(i)] = o;
    /* Do the same thing for o */
    for(i=0; i<3; ++i) if ( omatch[i] == -1 ) break;
    o->adjverts[MLTbl(i)] = r;
    o->is_root = 0;

    return 0;
}
int DetachVNode(VNode r, VNode o)
{
    assert(r); assert(o);

    return 0;
}
int AttachPoint(VNode r, MNode m, VNode* n, int face)
{
    assert(r); assert(m);
    assert(face < 3 && face >=0);
    /*
     face == 0 : between point 0 1
     face == 1 : between point 1 2
     face == 2 : between point 2 0
    */

    VNode nv;
    MNode o1, o2;

    switch (face) {
        case 0:
            o1 = r->points[0];
            o2 = r->points[1];
            break;
        case 1:
            o1 = r->points[1];
            o2 = r->points[2];
            break;
        case 2:
            o1 = r->points[2];
            o2 = r->points[0];
            break;
    }

    nv = MakeVNode(m, o1, o2);
    nv->adjverts[0] = r;
    r->adjverts[face] = nv;
    (*n) = nv;

    return 0;
}



/* Other utils */
