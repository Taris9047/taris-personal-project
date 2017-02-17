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
        AppendH(tmp_root, tmp);
        printf("First row %lu/%lu finished.\n",i+1,num_hor);
    }
    /* Then extend them to vertical direction */
    printf("Extending the first row vertically.\n");
    tmp_root = root;
    i = 1;
    while (tmp_root->rh) {
        for (j=1; j<num_ver; ++j) {
            d = NewDataAt((double)i, (double)j);
            tmp = NewMeshData(d);
            AppendV(tmp_root, tmp);
        }
        printf("Column %lu finished. (%lu/%lu)\n",i+1, i+1, num_hor);
        ++i;
        tmp_root = tmp_root->rh;
    }

    unsigned long xlen;
    unsigned long ylen;
    MeshNodesXY(root, &xlen, &ylen);
    printf("X direction: %lu nodes.\n", xlen);
    printf("y direction: %lu nodes.\n", ylen);
    printf("Total number of nodes: %lu\n", xlen*ylen);

    tmp_root = root;
    unsigned long cnt = 0;
    MeshTravAll(&tmp_root, &cnt);
    printf("Traverse all method: %lu\n", cnt);

    printf("Ok, destroying the mesh\n\n");
    MeshDestroy(root);

    printf("Test finished!!\n");
    printf("\n");

    return 0;
}
