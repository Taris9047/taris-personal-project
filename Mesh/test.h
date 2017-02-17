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

#endif /* Include guard */
