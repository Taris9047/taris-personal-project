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
#include "graph.h"
#include "test.h"

#define NUM_HORIZONTAL 100
#define NUM_VERTICAL 100

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
    printf("Mesh generation test program\n\n");

    unsigned long num_hor;
    unsigned long num_ver;

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

    /* test functions */
    test_mesh(num_hor, num_ver);
    test_graph(num_hor, num_ver);

    printf("\n");
    printf("Test finished!!\n");
    printf("\n");

    return 0;
}

/* Test functions */
/* mesh test */
void test_mesh(unsigned long nh, unsigned long nv)
{
    /* Lets test it!! */
    MNode root;
    MNode tmp;
    MNode tmp_root;
    MNode tmp_v;
    Data** d;
    unsigned long num_hor, num_ver;
    num_hor = nh; num_ver = nv;
    unsigned long i;
    unsigned long j;

    printf("[Mesh] *** 2D mesh test ***\n");
    printf("[Mesh] Generating a rectangular mesh with X: %lu and Y: %lu nodes.\n\n", num_hor, num_ver);

    /* Making a rectangular mesh */
    /* let's generate a data set */
    d = (Data**)malloc(sizeof(Data*)*nh);
    assert(d);
    for (i=0; i<nh; ++i) {
        d[i] = (Data*)malloc(sizeof(Data)*nv);
        assert(d[i]);
    }
    for (i=0; i<nh; ++i) {
        for (j=0; j<nv; ++j) {
            d[i][j] = NewDataAt((double)i, (double)j);
        }
    }

    /* making the first node */
    root = NewMeshData(d[0][0]);
    /* make first row */
    printf("[Mesh] Making first row...\n");
    tmp_root = root;
    for (i=1; i<nh; ++i) {
        tmp = NewMeshData(d[i][0]);
        PushH(&tmp_root, tmp);
        ProgressBar(i+1, nh, "[Mesh] First Row Progress");
    }
    printf("\n");
    /* update root since we pushed in a lot */
    MeshFindRoot(&root);

    /* Then extend them to vertical direction */
    printf("[Mesh] Extending the first row vertically.\n");
    tmp_root = root;
    tmp_v = root;
    for (i=0; i<nh; ++i) {
        for (j=1; j<nv; ++j) {
            tmp = NewMeshData(d[i][j]);
            PushV(&tmp_root, tmp);
        }
        ProgressBar(i+1, nh, "[Mesh] Column Progress");
        tmp_root = tmp_v->rh;
        tmp_v = tmp_v->rh;
    }
    printf("\n");

    MeshFindRoot(&root);

    unsigned long xlen;
    unsigned long ylen;
    MeshNodesXY(root, &xlen, &ylen);
    printf("[Mesh] X direction: %lu nodes.\n", xlen);
    printf("[Mesh] y direction: %lu nodes.\n", ylen);
    printf("[Mesh] Total number of nodes: %lu\n", xlen*ylen);

    unsigned long cnt = 0;
    MeshTravAllVerbose(&root, &cnt);
    printf("[Mesh] Traverse all method: %lu\n", cnt);

    printf("[Mesh] Ok, destroying the mesh\n\n");
    MeshDestroy(root);

    /* free data array */
    for (i=0; i<nh; ++i)
        for (j=0; j<nv; ++j) free(d[i][j]);
    for (i=0; i<nh; ++i) free(d[i]);
    free(d);

}


/* Graph test */
void test_graph(unsigned long nh, unsigned long nv)
{
    printf("[GTest] *** Testing graph generation ***\n");

    GNode tgr = InitGraph();
    GNode tgr_r_b = tgr;
    GNode tmpgr_h;
    GNode tmpgr_v;
    Data** tmp_data_ary;
    unsigned long i, j;
    unsigned long num_g;

    /* prepare data */
    printf("[GTest] Assigning memory space for dummy data.\n");
    tmp_data_ary = (Data**)malloc(sizeof(Data*)*nh);
    assert(tmp_data_ary);
    for (i=0; i<nh; ++i) {
        tmp_data_ary[i] = (Data*)malloc(sizeof(Data)*nv);
        assert(tmp_data_ary[i]);
        ProgressBar(i+1, nh, "[GTest] Progress");
    }
    printf("\n");
    printf("[GTest] Now assigning data to dummy data space.\n");
    for (i=0; i<nh; ++i) {
        for (j=0; j<nv; ++j) {
            tmp_data_ary[i][j] = NewDataAt((double)i, (double)j);
        }
        ProgressBar(i+1, nh, "[GTest] Progress");
    }
    printf("\n");

    /* Make some crappy graph structure with the 2D dataset */
    printf("[GTest] Making graph ...\n");
    tmpgr_h = tgr;
    for (i=0; i<nh; ++i) {
        if (i==0) tgr->data = tmp_data_ary[0][0];
        else {
            tmpgr_h = InitGraphData(tmp_data_ary[i][0]);
            GraphAttach(tgr, tmpgr_h);
        }
        if (i > 0) tgr = tmpgr_h;

        for (j=1; j<nh; ++j) {
            tmpgr_v = InitGraphData(tmp_data_ary[i][j]);
            GraphAttach(tgr, tmpgr_v);
            tgr = tmpgr_v;
        }
        tgr = tmpgr_h;
    }
    tgr = tgr_r_b;

    printf("\n");

    printf("[GTest] Testing graph traverse\n");
    GraphTraverse(&tgr, &num_g);
    printf("[GTest] Found nodes during traverse: %lu\n", num_g);

    printf("[GTest] Destroying graph\n");
    GraphDestroy(tgr);

    printf("[GTest] Cleaning up dummy data...\n");
    for (i=0; i<nh; ++i)
        for (j=0; j<nv; ++j)
            free(tmp_data_ary[i][j]);
    for (i=0; i<nh; ++i) free(tmp_data_ary[i]);
    free(tmp_data_ary);
}



/* Static functions */

/* Rectangular traverse */
static int MeshTravAllVerbose(MNode* m, unsigned long* n_tot)
{
    assert(*m);

    printf("\n");
    const char* txt = "[Mesh] Mesh Traverse All Progress";

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
    printf("%s: %lu/%lu (%d %%)", header_txt, curr, all, (int)(curr/all*100));
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
