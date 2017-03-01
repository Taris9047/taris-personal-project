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

#define TEST_PROGRAM

#include "list.h"
#include "meshgen.h"
#include "graph.h"
#include "test.h"
#include "utils.h"

#define NUM_HORIZONTAL 200
#define NUM_VERTICAL 150


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


Data** MakeNewDummyData(unsigned long rows, unsigned long cols)
{
    assert(rows > 0 && cols > 0);

    unsigned long i, j;
    Data** nd = (Data**)malloc(sizeof(Data*)*rows);
    assert(nd);
    for (i=0; i<rows; ++i) {
        nd[i] = (Data*)malloc(sizeof(Data)*cols);
        assert(nd[i]);
    }

    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            nd[i][j] = NewDataAt((double)i, (double)j);
        }
    }

    return nd;
}

void DestroyDummyData(Data** d, unsigned long rows, unsigned long cols)
{
    assert(d);

    unsigned long i, j;
    for (i=0; i<rows; ++i) {
        for (j=0; j<cols; ++j) {
            free(d[i][j]);
        }
    }

    for (i=0; i<rows; ++i) free(d[i]);
    free(d);
}


/* The main function */
int main(int argc, char* argv[])
{
    printf("*********************************\n");
    printf("*                               *\n");
    printf("* Mesh generation test program. *\n");
    printf("* Ver 0.0.1                     *\n");
    printf("*                               *\n");
    printf("*********************************\n");
    printf("\n");

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
    test_mesh_manip(num_hor, num_ver, num_hor/2, num_ver/2);
    printf("[Mesh] Mesh test finished ... \n\n");

    test_graph(num_hor, num_ver);

    printf("\n\n");
    printf("Test finished!!\n");
    printf("\n\n");

    return 0;
}

/* Test functions */
/* mesh test */
void test_mesh(unsigned long nh, unsigned long nv)
{
    /* Lets test it!! */
    MNode root;
    Data** d;

    printf("[Mesh] *** 2D mesh test ***\n");
    printf("[Mesh] Generating a rectangular mesh with X: %lu and Y: %lu nodes.\n\n", nh, nv);

    /* Making a rectangular mesh */
    /* let's generate a data set */
    d = MakeNewDummyData(nh, nv);
    root = MeshMakeRect(nh, nv);
    MeshSetData(root, (mesh_data_t**)d, nh, nv);

    MeshFindRoot(&root);

    unsigned long xlen, ylen;
    MeshNodesXY(root, &xlen, &ylen);
    printf("[Mesh] X direction: %lu nodes.\n", xlen);
    printf("[Mesh] y direction: %lu nodes.\n", ylen);
    printf("[Mesh] Total number of nodes: %lu\n", xlen*ylen);

    unsigned long cnt = 0;
    MeshTravAllVerbose(&root, &cnt);
    printf("[Mesh] Traverse all method: %lu\n", cnt);

    printf("[Mesh] Ok, destroying the mesh\n\n");
    MeshDestroy(root, nh, nv);

    /* free data array */
    DestroyDummyData(d, nh, nv);
}

/* mesh manipulation test */
void test_mesh_manip(unsigned long ar, unsigned long ac, unsigned long br, unsigned long bc)
{
    assert( ar >= 2 && ac >= 2 && br >= 1 && bc >= 1);

    printf("[Mesh] *** Testing mesh manipulation ***\n");

    Data** data_a = MakeNewDummyData(ar, ac);
    Data** data_b = MakeNewDummyData(br, bc);
    MNode A, B, tmp;
    unsigned long* n_tot;

    n_tot = (unsigned long*)malloc(sizeof(unsigned long));

    printf("[Mesh] Making meshes (using meshgen libraries)\n");
    A = MeshMakeRect(ar, ac);
    MeshSetData(A, (mesh_data_t**)data_a, ar, ac);
    printf("[Mesh] A has %lu rows and %lu columns.\n", ar, ac);
    printf("[Mesh] Traversing all of A\n");
    tmp = A;
    MeshTravAllVerbose(&tmp, n_tot);

    B = MeshMakeRect(br, bc);
    MeshSetData(B,(mesh_data_t**) data_b, br, bc);
    printf("[Mesh] B has %lu rows and %lu columns.\n", br, bc);
    printf("[Mesh] Traversing all of B\n");
    tmp = B;
    MeshTravAllVerbose(&tmp, n_tot);

    printf("[Mesh] Attaching two meshes ...\n");
    MeshAttachRHU(A, B);

    printf("[Mesh] Checking the attached mesh ...\n");

    //MeshTravRect(&tmp, n_tot);
    //printf("[Mesh] Found meshes: %lu\n", *n_tot);
    tmp = A;
    MeshTravAllVerbose(&tmp, n_tot);

    printf("[Mesh] Destroying attached mesh ... \n");
    //MeshDestroyAll(A);
    MeshDestroy(A, ar, ac);
    MeshDestroy(B, br, bc);

    DestroyDummyData(data_a, ar, ac);
    DestroyDummyData(data_b, br, bc);
    free(n_tot);
    printf("[Mesh] Mesh manipulation test done.\n");
    return;
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
    printf("[GTest] Making dummy data.\n");
    tmp_data_ary = MakeNewDummyData(nh, nv);

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

    printf("[GTest] Testing graph traverse\n");
    GraphTraverse(&tgr, &num_g);
    printf("[GTest] Found nodes during traverse: %lu\n", num_g);

    printf("[GTest] Destroying graph\n");
    GraphDestroy(tgr);

    printf("[GTest] Cleaning up dummy data ...\n");
    DestroyDummyData(tmp_data_ary, nh, nv);
}
