/**
* @Author: Taylor Shin <kshin>
* @Date:   2017-02-17T10:43:57-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   kshin
* @Last modified time: 2017-02-17T13:59:58-06:00
*/
/***************************************

  Mesh Generation Library
  Test program

  Header file

  Taylor Shin, Feb. 17 2017

 ***************************************/
#ifndef TEST_H
#define TEST_H

#include "utils.h"

typedef struct _dummy_data {
    double x_coord;
    double y_coord;

    double efield;
    double potential;

    double impurity;
    double eff_mass;
} data;
typedef data* Data;

/* Make a dummy data */
Data NewData();
Data NewDataAt(const double x, const double y);
Data** MakeNewDummyData(unsigned long rows, unsigned long cols);
void DestroyDummyData(Data** d, unsigned long rows, unsigned long cols);

/* Test Functions */
void test_graph(unsigned long nh, unsigned long nv);
void test_mesh(unsigned long nh, unsigned long nv);
void test_mesh_manip(unsigned long ar, unsigned long ac, unsigned long br, unsigned long bc);

#endif /* Include guard */
