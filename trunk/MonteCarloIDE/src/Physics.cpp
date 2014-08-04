#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include "Molecule.hpp"
#include "Physics.hpp"

#ifndef PHYSICS_CPP
#define PHYSICS_CPP

using namespace std;

// Methods

void Physics::advance_time()
{
	this->x_loc.push_back(this->x_loc.back()+\
		this->time_scale*this->x_vel.back());
	this->y_loc.push_back(this->y_loc.back()+\
		this->time_scale*this->y_vel.back());

	this->x_vel.push_back(0.0);
	this->y_vel.push_back(0.0);

	this->time_eclipsed += this->time_scale;
	this->time_trace.push_back(time_eclipsed);

}

void Physics::brownian_rect(float max_vel_x, float max_vel_y)
{
	srand(time(0));
	if (this->x_vel.back() == 0.0 && this->y_vel.back() == 0.0) {
		this->x_vel.pop_back();
		this->x_vel.push_back((float)rand()/max_vel_x);
		this->y_vel.pop_back();
		this->y_vel.push_back((float)rand()/max_vel_y);
	}

	this->x_loc.push_back(this->x_loc.back()+\
		this->time_scale*this->x_vel.back());
	this->y_loc.push_back(this->y_loc.back()+\
		this->time_scale*this->y_vel.back());
	this->x_vel.push_back((float)rand()/max_vel_x);
	this->y_vel.push_back((float)rand()/max_vel_y);

	this->time_eclipsed += this->time_scale;
	this->time_trace.push_back(time_eclipsed);
}

void Physics::reflect_rect(float edge_x, float edge_y)
{

	if (this->x_loc.back() > edge_x ||
		this->y_loc.back() > edge_y) {
		throw invalid_argument( \
			"The trace already found at the outside of \
			the boundary. Cannot proceed!!\n\n");
		exit(1);
	}

	float div_time = 0.0; // Must be smaller than time scale.
	float div_time_left = time_scale - div_time;
	float div_ratio;

	do {
		// Dealing with X axis first
		float next_x = this->x_loc.back()+\
			this->x_vel.back()*div_time_left;
		float next_y = this->y_loc.back()+\
			this->y_vel.back()*div_time_left;

		if (next_x > edge_x || next_x < edge_x*(-1)) {
			float offset_x;
			float div_ratio_x;
			if (next_x > edge_x) {
				offset_x = next_x - edge_x;
			}
			else if (next_x < edge_x*(-1)) {
				offset_x = edge_x*(-1) - next_x;
			}
			// continue this crap


		}

	} while (div_time <= time_scale);

}



// Constructors and Destructors

Physics::Physics()
{
	this->time_eclipsed = 0.0;
	this->time_scale = 1.0;
	this->curr_object = NULL;

	this->obj_mass = 0.0;

	this->time_trace.push_back(0);
}

Physics::Physics(Molecule* Thing)
{
	this->time_eclipsed = 0.0;
	this->time_scale = 1.0;
	this->curr_object = Thing;

	vector<float> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(0);
}

Physics::Physics(float time_ec, float time_sc, Molecule* Thing)
{
	this->time_eclipsed = time_ec;
	this->time_scale = time_sc;
	this->curr_object = Thing;

	vector<float> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(0);

}

Physics::~Physics()
{
	this->x_loc.clear();
	this->y_loc.clear();
	this->x_vel.clear();
	this->y_vel.clear();

	this->time_trace.clear();
}

#endif
