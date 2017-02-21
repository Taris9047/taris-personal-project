/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-16T13:46:41-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T13:59:33-06:00
*/
/***************************************

  Mesh Generation Library
  Test program

  Taylor Shin, Feb. 16 2017

 ***************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"
#include "meshgen.h"
#include "test.h"

#define NUM_HORIZONTAL 250
#define NUM_VERTICAL 250

/* Data constructors */
Data NewData()
{
    Data n = (Data)malloc(sizeof(data));
    assert(n);

    n->x_coord = 0.0;
    n->y_coord = 0.0;

    n->efield = 0.0;
    n->potential = 0.0;

    n->impurity = 0.0;
    n->eff_mass = 0.0;

    return n;
}
Data NewDataAt(const double x, const double y)
{
    Data n = NewData();
    n->x_coord = x;
    n->y_coord = y;

    n->efield = (double)rand();
    n->potential = (double)rand();

    n->impurity = (double)rand();
    n->eff_mass = (double)rand();

    return n;
}


/* The main function */
int main(int argc, char* argv[])
{
    printf("Mesh generation test program\n");

    unsigned long num_hor;
    unsigned long num_ver;
    unsigned long i;
    unsigned long j;

    /* Check inputs.. gets only two inputs */
    if (argc == 3) {
        num_hor = (unsigned long)atoi(argv[1]);
        num_ver = (unsigned long)atoi(argv[2]);
    }
    else {
        num_hor = (unsigned long)NUM_HORIZONTAL;
        num_ver = (unsigned long)NUM_VERTICAL;
    }

    int t = time(NULL);
    srand(t);

    printf("Generating a rectangular mesh with X: %lu and Y: %lu nodes.\n\n", num_hor, num_ver);

    /* Lets test it!! */
    MNode root;
    MNode tmp;
    MNode tmp_root;
    MNode tmp_v;
    Data d;

    /* Making a rectangular mesh */
    /* making the first node */
    d = NewDataAt(0.0, 0.0);
    root = NewMeshData(d);
    /* make first row */
    printf("Making first row...\n");
    tmp_root = root;
    for (i=1; i<num_hor; ++i) {
        d = NewDataAt((double)i, (double)0.0);
        tmp = NewMeshData(d);
        PushH(&tmp_root, tmp);
        printf("First row %lu/%lu finished.",i+1,num_hor);
        printf("\r");
        fflush(stdout);
    }
    printf("\n");
    /* update root since we pushed in a lot */
    MeshFindRoot(&root);

    /* Then extend them to vertical direction */
    printf("Extending the first row vertically.\n");
    tmp_root = root;
    tmp_v = root;
    i = 1;
    while (tmp_v) {
        for (j=1; j<num_ver; ++j) {
            d = NewDataAt((double)i, (double)j);
            tmp = NewMeshData(d);
            PushV(&tmp_root, tmp);
        }
        printf("Column %lu finished. (%lu/%lu)", i, i, num_hor);
        printf("\r");
        fflush(stdout);
        ++i;
        tmp_root = tmp_v->rh;
        tmp_v = tmp_v->rh;
    }
    printf("\n");

    MeshFindRoot(&root);

    unsigned long xlen;
    unsigned long ylen;
    MeshNodesXY(root, &xlen, &ylen);
    printf("X direction: %lu nodes.\n", xlen);
    printf("y direction: %lu nodes.\n", ylen);
    printf("Total number of nodes: %lu\n", xlen*ylen);

    unsigned long cnt = 0;
    MeshTravAllVerbose(&root, &cnt);
    printf("Traverse all method: %lu\n", cnt);

    printf("Ok, destroying the mesh\n\n");
    MeshDestroy(root);

    printf("Test finished!!\n");
    printf("\n");

    return 0;
}


/* Rectangular traverse */
static int MeshTravAllVerbose(MNode* m, unsigned long* n_tot)
{
    assert(*m);

    printf("\n");
    const char* txt = "Mesh Traverse All Progress";

    MNode tmp = (*m);
    (*n_tot) = 1; /* self */
    LNode ml = ListInit(); /* Initialize list of meshes ... as linked list */
    ListPush(&ml, tmp); /* Push in first node */
    ProgressNum((*n_tot), txt);

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
            ProgressNum((*n_tot), txt);
            continue;
        }

        if (tmp->rd && !ListFind(ml, tmp->rd)) {
            ListPush(&ml, tmp->rd);
            tmp = tmp->rd;
            ++(*n_tot);
            ProgressNum((*n_tot), txt);
            continue;
        }

        if (tmp->dn && !ListFind(ml, tmp->dn)) {
            ListPush(&ml, tmp->dn);
            tmp = tmp->dn;
            ++(*n_tot);
            ProgressNum((*n_tot), txt);
            continue;
        }

        if (tmp->lh && !ListFind(ml, tmp->lh)) {
            ListPush(&ml, tmp->lh);
            tmp = tmp->lh;
            ++(*n_tot);
            ProgressNum((*n_tot), txt);
            continue;
        }

        if (tmp->up && !ListFind(ml, tmp->up)) {
            ListPush(&ml, tmp->up);
            tmp = tmp->up;
            ++(*n_tot);
            ProgressNum((*n_tot), txt);
            continue;
        }

        (*m) = tmp;
        break;
    }
    printf("\n");

    ListDestroy(ml);
    return 0;
}

static int ProgressBar(unsigned long curr, unsigned long all, const char* header_txt)
{
    printf("%s: %lu/%lu (%.0f)", header_txt, curr, all, (double)(curr/all));
    printf("\r");
    fflush(stdout);

    return 0;
}
static int ProgressNum(unsigned long curr, const char* header_txt)
{
    printf("%s: %lu", header_txt, curr);
    printf("\r");
    fflush(stdout);

    return 0;
}
