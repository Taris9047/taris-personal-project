#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "data_types.hpp"
#include "Molecule.hpp"

// Physics implementation

// Currently, contains only reflection handler.

class Physics: public Molecule
{
private:
	// Setting up time and time scale.
	float time_elapsed;
	float time_limit;
	float time_scale;
	// current object: Molecule (pointer)
	Molecule* curr_object;

	// Logging trace of the object.
	std_vec_f x_loc;
	std_vec_f y_loc;
	std_vec_f x_vel;
	std_vec_f y_vel;
	std_vec_b reflected_status;
	std_vec_f time_trace;

	// Add more constants if other coordinate system has used.

	float obj_mass;

	std_vec_f proj_loc_rect(float time_segment); // returns coordinate after a certain time segment.
	
	float rand_float(float min, float max);
	void update_status(float x, float y, float vx, float vy, \
		bool refl, float curr_time);
	void log_status();
	void advance_time();
	std_str bool_to_yesno(bool logic);

	// Report Status
	void print_status_rect();

public:
	// reflecting in rectangular system
	void reflect_rect(float edge_left, float edge_right, \
		float edge_top, float edge_bottom, \
		float time_frame);
	void brownian_rect(float max_vel_x, float max_vel_y, \
		float edge_left, float edge_right, \
		float edge_top, float edge_bottom);

    // Write current history into a file
	void write_log_rect(std_str outfile_name); 

	// Constructures and Destructors
	Physics();
	Physics(Molecule* Thing);
	Physics(float time_limit, float time_sc, Molecule* Thing);
	~Physics();

};

#endif
