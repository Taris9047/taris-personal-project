#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include "data_types.hpp"
#include "Molecule.hpp"
#include "Photon.hpp"
#include "Random.hpp"
#include "Converters.hpp"

// Physics implementation

// Currently, contains only reflection handler.

class Physics 
	: public virtual Random
{
private:
	// Setting up time and time scale.
	double time_elapsed;
	double time_limit;
	double time_scale;
	// current object: Molecule (pointer)
	Molecule* curr_object;
	Photon* curr_photon;
	unint rand_type;
	double edge_left;
	double edge_right;
	double edge_top;
	double edge_bottom;

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
	double rand_double(
		double paramA, double paramB, double paramC);
	void set_rand_velocity_rect(
		double paramA, double paramB, double paramC);
	void set_rand_velocity_rect_x(
		double paramA, double paramB, double paramC);
	void set_rand_velocity_rect_y(
		double paramA, double paramB, double paramC);

	// Utilities
	void update_status(
		double x, double y, \
		double vx, double vy, \
		bool refl);
	void log_status();
	void advance_time();
	void Validate_Data();

	// reflecting in rectangular system
	void reflect_rect(double time_frame);

public:

	// Brownian movement simulation
	void brownian_rect(double max_vel_x, double max_vel_y);

	// Normal movement simulation
	void normal_rect(double vel_x, double vel_y);

    // Write current history into a file
	void write_log_rect(
		std_str outfile_name, std_str cDelim, std_str linbreak); 
	// Extract current history with a std::string
	std_str extract_log_rect(std_str cDelim, std_str linbreak);

	// Set up dimension and etc.
	bool set_dimension_rect(
		double dim_left, double dim_right, \
		double dim_top,	double dim_bottom);
	bool set_parameters(bool verbose, unint rand_type);
	bool set_timing(double d_time_limit, double d_time_scale);
	bool set_Molecule(Molecule* Thing);
	bool set_Photon(Photon* pPhoton);
	void select_RNG(unint rng_type);
	void set_location_rect(double xloc, double yloc);
	
	// Utilities for public use
	std_str get_RNG_type();	
	void Reset_Sim();

	// Report Status
	void print_status_rect();
	std_str sprint_status_rect(std_str linbreak);
	// Show simulation settings
	std_str show_siminfo_rect(std_str linbreak, bool use_RNG = "true");
	std_vec_d report_status_rect();
	std_str show_dimension_rect(std_str linbreak);
	
	// Constructures and Destructors
	Physics(Molecule* Thing);
	Physics(Molecule* Thing, bool verbose);
	Physics(Molecule* Thing, double time_lim, double time_sc);
	Physics(Molecule* Thing, double time_lim, double time_sc, \
		bool verbose);
	Physics(Molecule* Thing, double time_lim, double time_sc, \
		bool verbose, unint rnd_type);
	Physics(Molecule* Thing, double time_lim, double time_sc, \
		double rect_left, double rect_right, \
		double rect_top, double rect_bottom, \
		bool verbose, unint rnd_type);
	~Physics();

};

#endif
