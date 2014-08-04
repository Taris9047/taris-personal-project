#include "Molecule.hpp"

#ifndef PHYSICS_HPP
#define PHYSICS_HPP

using namespace std;

// Physics implementation

// Currently, contains only reflection handler.

class Physics: public Molecule
{
private:
	// Setting up time and time scale.
	float time_eclipsed;
	float time_scale;
	// current object: Molecule (pointer)
	Molecule* curr_object;

	// Logging trace of the object.
	vector<float> x_loc;
	vector<float> y_loc;
	vector<float> x_vel;
	vector<float> y_vel;
	vector<float> time_trace;

	// Add more constants if other coordinate system has used.

	float obj_mass;

public:
	// reflecting in rectangular system
	void reflect_rect(float edge_x, float edge_y);
	void brownian_rect(float max_vel_x, float max_vel_y);

	void advance_time();

	// Constructures and Destructors
	Physics();
	Physics(Molecule* Thing);
	Physics(float time_ec, float time_sc, Molecule* Thing);
	~Physics();

};

#endif