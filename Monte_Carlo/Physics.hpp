#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "data_types.hpp"
#include "Molecule.hpp"
#include "Random.hpp"

// Physics implementation

// Currently, contains only reflection handler.

class Physics: public Molecule, public Random
{
private:
	// Setting up time and time scale.
	double time_elapsed;
	double time_limit;
	double time_scale;
	// current object: Molecule (pointer)
	Molecule* curr_object;

	// Verbose
	bool b_verbose;

	// Logging trace of the object.
	std_vec_d x_loc;
	std_vec_d y_loc;
	std_vec_d x_vel;
	std_vec_d y_vel;
	std_vec_b reflected_status;
	std_vec_d time_trace;

	// Add more constants if other coordinate system has used.
	double obj_mass;

	// returns coordinate after a certain time segment.
	std_vec_d proj_loc_rect(double time_segment); 	
	double rand_double(double min, double max);
	void update_status(double x, double y, \
		double vx, double vy, \
		bool refl, double curr_time);
	void log_status();
	void advance_time();
	std_str bool_to_yesno(bool logic);

	// Report Status
	void print_status_rect();
	std_vec_d report_status_rect();

	// reflecting in rectangular system
	void reflect_rect(double edge_left, double edge_right, \
		double edge_top, double edge_bottom, \
		double time_frame);

public:

	void brownian_rect(double max_vel_x, double max_vel_y, \
		double edge_left, double edge_right, \
		double edge_top, double edge_bottom);

    // Write current history into a file
	void write_log_rect(std_str outfile_name); 

	// Constructures and Destructors
	Physics();
	Physics(bool verbose);
	Physics(Molecule* Thing);
	Physics(Molecule* Thing, bool verbose);
	Physics(double time_limit, double time_sc, Molecule* Thing);
	Physics(double time_limit, double time_sc,\
		Molecule* Thing, bool verbose);
	~Physics();

};

#endif
