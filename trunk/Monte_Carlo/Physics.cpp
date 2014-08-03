#include <cmath>
#include <iostream>
#include <vector>
#include "Physics.hpp"
#include "Molecule.hpp"

#ifndef PHYSICS_CPP
#define PHYSICS_CPP

using namespace std;

void Physics::reflect_rect(float edge_x, float edge_y, float time_scale, Molecule* Thing)
{
	vector<float> obj_info(5);
	obj_info = Thing->read_info();

	// Initialize conditions
	float curr_x, curr_y, curr_xv, curr_yv, next_x, next_y;
	curr_x = obj_info[0];
	curr_y = obj_info[1];
	curr_xv = obj_info[2];
	curr_yv = obj_info[3];

	next_x = curr_x + time_scale*next_x;
	next_y = curr_y + time_scale*next_y;



}

#endif