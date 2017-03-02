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

/* Direction mapping
    0 rh
    1 rd
    2 dn
    3 lh
    4 ul
    5 up
*/

/* Lefty mapping matrix */
static const int lefty_matrix[6][5] = \
{\
    {1, 2, 3, 4, 5},\
    {2, 3, 4, 5, 0},\
    {3, 4, 5, 0, 1},\
    {4, 5, 0, 1, 2},\
    {5, 0, 1, 2, 3},\
    {0, 1, 2, 3, 4}\
};

/* Static functions */

/* Update adjacent node information
   Returns 1 if nowhere to go */
static int AdjNodeInfo(MNode m, LNode ml, int nxt_found[6], int nxt_exist[6]);

/* Find left node */
static int FindLeft(MNode* m, MNode* p, LNode ml);

/* Numeric mapping for next node */
static int Map(MNode* m, int nxt);

/* returns prev direction */
static int MapPrev(MNode m, MNode p);

/* Dead end escape */
/* returns 0 if there's no escape */
static int Escape(MNode* m, LNode h);

/* Lefty pathing table reader */
/* Input: prev direction, next direction */
static inline int LTbl(int p_ind, int n_ind) { return lefty_matrix[p_ind][n_ind]; }

/* Traverse all */
static LNode TravAll(MNode m);


/* Constructors and Destructors */
MNode NewMesh()
{
    MNode nm = (MNode)malloc(sizeof(mesh_node));
    //if (!nm) return NULL;
    assert(nm);

    nm->data = NULL;
    nm->id = 0;
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

/* New mesh with data and id */
MNode NewMeshDataID(mesh_data_t data, unsigned long id)
{
    MNode nm = NewMeshData(data);
    nm->id = id;
    return nm;
}

/* Alias for NewMesh() */
MNode MeshInit()
{
    return NewMesh();
}

/* Rectangular traverse destructor */
int MeshDestroy(MNode m, unsigned long nh, unsigned long nv)
{
    MNode tmp;
    MNode tmp_h;

    unsigned long i, j;

    /* empty node, nothing to destroy */
    if (!m) return 0;
    i = j = 0;

    while (i<nh) {
        j = 0;
        tmp_h = m->rh;
        while (j<nv-1) {
            tmp = m;
            m = m->dn;
            free(tmp);
            ++j;
        }
        if (tmp_h) m = tmp_h;
        else break;
        ++i;
    }

    return 0;
}

/* Complete traverse destructor */
/* of course, m should be root, the top-left node */
int MeshDestroyAll(MNode m)
{
    assert(m);

    LNode ml = TravAll(m);
    LNode ml_tmp = ml;
    while (ml_tmp) {
        free((MNode)ml_tmp->value);
        ml_tmp = ml_tmp->next;
    }

    ListDestroy(ml);

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

    unsigned long i = 1;
    while((*m)->rh) {
        (*m) = (*m)->rh; ++i;
    }

    return i;
}

unsigned long MeshTravV(MNode* m)
{
    assert(*m);

    unsigned long i = 1;
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

    (*i) = 1;
    (*j) = 1;

    MNode tmp_v = (*m);

    while(tmp_v) {
        while((*m)->rh) {
            (*m) = (*m)->rh;
            ++(*i);
        }
        (*m) = tmp_v->dn;
        tmp_v = tmp_v->dn;
        (*i) = 1;
        ++(*j);
    }

    return 0;
}

/* Complete Traverse */
int MeshTravAll(MNode* m, unsigned long* n_tot)
{
    assert(*m);
    assert(n_tot);

    MNode tmp = (*m);
    MNode tmp_prev = NULL;
    (*n_tot) = 1; /* self */
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */

    int dead_end = 0;

    ListPush(&ml, tmp); /* Push in first node */
    while (1) {
        /* update adjacent node existence in history */
        dead_end = FindLeft(&tmp, &tmp_prev, ml);

        if (!dead_end) {
            ListPush(&ml, tmp);
            (*n_tot)++;
        }
        /* Dead end case */
        else {
            /* try to escape */
            if (!Escape(&tmp, ml)) break;
            else { /* Escaped!! */
                ListPush(&ml, tmp);
                (*n_tot)++;
                tmp_prev = NULL;
            }
        }
    }

    ListDestroy(ml);
    return 0;
}

/* Find by data */
MNode MeshFind(MNode m, mesh_data_t stuff)
{
    assert(m);

    MNode tmp = m;
    MNode tmp_prev = NULL;
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */

    int dead_end = 0;

    ListPush(&ml, tmp); /* Push in first node */
    while (1) {
        /* update adjacent node existence in history */
        dead_end = FindLeft(&tmp, &tmp_prev, ml);

        if (tmp->data == stuff) break;

        if (!dead_end) {
            ListPush(&ml, tmp);
        }
        /* Dead end case */
        else {
            /* try to escape */
            if (!Escape(&tmp, ml)) break;
            else { /* Escaped!! */
                ListPush(&ml, tmp);
                tmp_prev = NULL;
            }
        }
    }

    ListDestroy(ml);
    return tmp;
}

/* Find by ID */
MNode MeshFindID(MNode m, unsigned long id)
{
    assert(m);

    MNode tmp = m;
    MNode tmp_prev = NULL;
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */

    int dead_end = 0;

    ListPush(&ml, tmp); /* Push in first node */
    while (1) {
        /* update adjacent node existence in history */
        dead_end = FindLeft(&tmp, &tmp_prev, ml);

        if (tmp->id == id) break;

        if (!dead_end) {
            ListPush(&ml, tmp);
        }
        /* Dead end case */
        else {
            /* try to escape */
            if (!Escape(&tmp, ml)) break;
            else { /* Escaped!! */
                ListPush(&ml, tmp);
                tmp_prev = NULL;
            }
        }
    }

    ListDestroy(ml);
    return tmp;
}

/* Find Root */
int MeshFindRoot(MNode *m)
{
    assert(*m);

    MNode tmp = (*m);

    /* If it's root, just skip all the cruds */
    if (MeshIsRoot(tmp)) return 0;

    /* at first, travel down to bottom */
    while (tmp->dn) tmp = tmp->dn;

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

/* Update adjacent node information */
static int AdjNodeInfo(MNode m, LNode ml, int nxt_found[6], int nxt_exist[6])
{
    assert(m);
    assert(nxt_found);
    assert(nxt_exist);

    int i;

    MNode tmp = m;

    if (tmp->rh) {
        nxt_exist[0] = 1;
        nxt_found[0] = ListFind(ml, tmp->rh);
    }
    if (tmp->rd) {
        nxt_exist[1] = 1;
        nxt_found[1] = ListFind(ml, tmp->rd);
    }
    if (tmp->dn) {
        nxt_exist[2] = 1;
        nxt_found[2] = ListFind(ml, tmp->dn);
    }
    if (tmp->lh) {
        nxt_exist[3] = 1;
        nxt_found[3] = ListFind(ml, tmp->lh);
    }
    if (tmp->ul) {
        nxt_exist[4] = 1;
        nxt_found[4] = ListFind(ml, tmp->ul);
    }
    if (tmp->up) {
        nxt_exist[5] = 1;
        nxt_found[5] = ListFind(ml, tmp->up);
    }

    /* checking if all the other paths are found */
    for (i=0; i<6; ++i) {
        if (nxt_exist[i] && !nxt_found[i]) return 0;
    }
    /* If history list has all the adjacent nodes,
       this is a dead end...
       return true...
    */
    return 1;
}


/* Find left node */
static int FindLeft(MNode* m, MNode* p, LNode ml)
{
    assert(*m);
    assert(ml);

    MNode curr = (*m);
    MNode prev = (*p);
    MNode tmp = NULL;
    int i;
    int prev_dir;
    int next_dir;

    /* Checking if connected nodes are in history */
    /* nxt_found and nxt_exist has 6 slots:
        nxt_found[0] is rh
        nxt_found[1] is rd
        nxt_found[2] is dn
        nxt_found[3] is lh
        nxt_found[4] is ul
        nxt_found[5] is up
    */
    int nxt_found[6] = {0, 0, 0, 0, 0, 0};
    int nxt_exist[6] = {0, 0, 0, 0, 0, 0};
    int dead_end = AdjNodeInfo(curr, ml, nxt_found, nxt_exist);

    /* dead end... no point to continue */
    if (dead_end) return 1;

    if (prev == curr) return 1;

    /* If we don't have previous node ...
       Just redirect to any node we can see. */
    if (!prev) {
        tmp = curr;
        for (i=0; i<6; ++i) {
            if (nxt_exist[i] && !nxt_found[i]) {
                Map(&tmp, i);
                break;
            }
        }

        if (tmp == curr)
            return 1;
        else {
            (*p) = curr;
            curr = tmp;
            (*m) = curr;
            return 0;
        }
    }

    /* The lefty finder */
    /*
        Mapping table:
        prev 0 -> 1 2 3 4 5
        prev 1 -> 2 ... 5 0
        prev 2 -> 3 4 5 0 1
        prev 3 -> 4
        ... (const int lefty_matrix[6][5])
    */
    prev_dir = MapPrev(curr, prev);
    assert(prev_dir >= 0);
    tmp = curr;

    for (i=0; i<5; ++i) {
    	next_dir = LTbl(prev_dir, i);
    	if (nxt_exist[next_dir] && !nxt_found[next_dir]) {
    		Map(&tmp, next_dir);
    		break;
    	}
    }

    /* Failed to find a way */
    if (tmp == curr)
        return 1;
    else {
        (*p) = curr;
        curr = tmp;
        (*m) = curr;
        return 0;
    }
}

/* Numeric mapping for next node */
static int Map(MNode* m, int nxt)
{
    /*
       1st rh
       2nd rd
       3rd dn
       4th lh
       5th ul
       6th up
    */
    assert(*m);

    if (nxt == 0) (*m) = (*m)->rh;
    else if (nxt == 1) (*m) = (*m)->rd;
    else if (nxt == 2) (*m) = (*m)->dn;
    else if (nxt == 3) (*m) = (*m)->lh;
    else if (nxt == 4) (*m) = (*m)->ul;
    else if (nxt == 5) (*m) = (*m)->up;

    return nxt;
}

/* Returns direction code (0-5) of prev */
static int MapPrev(MNode m, MNode p)
{
    assert(m);

    if (m->rh == p) return 0;
    else if (m->rd == p) return 1;
    else if (m->dn == p) return 2;
    else if (m->lh == p) return 3;
    else if (m->ul == p) return 4;
    else if (m->up == p) return 5;
    else return -1;
}

/* Dead end escape */
/* returns 0 if there's no escape */
static int Escape(MNode* m, LNode h)
{
    assert(*m);
    assert(h);

    MNode curr = (*m);
    /* Let's start the unpassed node scan from the root */
    MeshFindRoot(&curr);
    MNode tmp_h = curr;

    while (tmp_h) {
        while (curr) {
            /*
               If we found unpassed node
               during search, update *m and escape!
            */
            if (!ListFind(h, curr)) {
                (*m) = curr;
                return 1;
            }
            curr = curr->rh;
        }
        curr = tmp_h->dn;
        tmp_h = tmp_h->dn;
    }

    if (!curr && !tmp_h) return 0;
    else return -1;
}

/* Traverse all and returns linked list */
static LNode TravAll(MNode m)
{
    assert(m);

    MNode tmp = m;
    MNode tmp_prev = NULL;
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */

    int dead_end = 0;

    ListPush(&ml, tmp); /* Push in first node */
    while (1) {
        /* update adjacent node existence in history */
        dead_end = FindLeft(&tmp, &tmp_prev, ml);

        if (!dead_end) {
            ListPush(&ml, tmp);
        }
        /* Dead end case */
        else {
            /* try to escape */
            if (!Escape(&tmp, ml)) break;
            else { /* Escaped!! */
                ListPush(&ml, tmp);
                tmp_prev = NULL;
            }
        }
    }

    return ml;
}

#ifdef TEST_PROGRAM
/* for debug */
/* Rectangular traverse */
int MeshTravAllVerbose(MNode* m, unsigned long* n_tot)
{
    assert(*m);
    assert(n_tot);

    const char* txt = "[Mesh] Mesh Traverse All Progress";

    MNode tmp = (*m);
    MNode tmp_prev = NULL;
    (*n_tot) = 1; /* self */
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */

    int dead_end = 0;

    ListPush(&ml, tmp); /* Push in first node */
    while (1) {
        /* update adjacent node existence in history */
        dead_end = FindLeft(&tmp, &tmp_prev, ml);

        if (!dead_end) {
            ListPush(&ml, tmp);
            (*n_tot)++;
        }
        /* Dead end case */
        else {
            // ListPush(&ml, tmp);
            // (*n_tot)++;
            /* try to escape */
            if (!Escape(&tmp, ml)) break;
            else {
                ListPush(&ml, tmp);
                (*n_tot)++;
                tmp_prev = NULL;
            }
        }
        ProgressNum((*n_tot), txt);
    }
    printf("\n");

    ListDestroy(ml);
    return 0;
}
#endif
