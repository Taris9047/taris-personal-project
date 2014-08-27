#ifndef PHYSICS_CPP
#define PHYSICS_CPP

#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include "Physics.hpp"
#include "Random.hpp"

using namespace std;

//////////////////////////////
//          Methods         //
//////////////////////////////
void Physics::advance_time()
{
	this->x_loc.push_back(this->x_loc.back()+\
		this->time_scale*this->x_vel.back());
	this->y_loc.push_back(this->y_loc.back()+\
		this->time_scale*this->y_vel.back());

	this->x_vel.push_back(0.0);
	this->y_vel.push_back(0.0);

	this->curr_object->set_location( \
		this->x_loc.back(),this->y_loc.back());
	this->curr_object->set_velocity( \
		this->x_vel.back(),this->y_vel.back());

	this->time_elapsed += this->time_scale;
	this->time_trace.push_back(this->time_elapsed);
	this->reflected_status.push_back(false);

	if (this->b_verbose == true) {
		this->print_status_rect();
	}
}

void Physics::brownian_rect(double max_vel_x, double max_vel_y)
{
	std_vec_d proj_loc(2);
	double time_frame;

	if (this->b_verbose) {
		show_dimension_rect("\n");
		cout << "Initial Condition:" << endl;
		print_status_rect();
	}
	// Initializing the first point with a random velocity vectors.
	this->set_rand_velocity_rect(
		max_vel_x / 2, max_vel_y / 3, (max_vel_x+max_vel_y)/2);

	// Log status into the class.
	this->log_status();

	do {
		// Running reflectance for this vector.
		if ( (this->time_elapsed + this->time_scale) > this->time_limit )
		   	time_frame = this->time_limit - this->time_elapsed;
        	if ( time_frame == 0 ) break;
        else 
        	time_frame = this->time_scale;
        
		proj_loc = proj_loc_rect(time_frame);

		if (proj_loc[0] < this->edge_left || proj_loc[0] > this->edge_right \
			|| proj_loc[1] < this->edge_bottom || proj_loc[1] > this->edge_top)
			this->reflect_rect(time_frame);
		else {
            this->curr_object->set_location(proj_loc[0], proj_loc[1]);
            this->time_elapsed += time_frame;
		}

		if (this->b_verbose == true)
			this->print_status_rect();

		this->set_rand_velocity_rect(
			max_vel_x / 2, max_vel_y / 3, (max_vel_x+max_vel_y)/2);

		this->log_status();

	} while (this->time_elapsed <= this->time_limit);
}

// Setting random velocity
void Physics::set_rand_velocity_rect(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	this->curr_object->set_velocity(
		this->rand_double(paramA, paramB, paramC), \
		this->rand_double(paramA, paramB, paramC));
}

// Setting random velocity for each axis
void Physics::set_rand_velocity_rect_x(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	this->curr_object->set_xv(
		this->rand_double(paramA, paramB, paramC));
}

// Setting random velocity for each axis
void Physics::set_rand_velocity_rect_y(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	this->curr_object->set_yv(
		this->rand_double(paramA, paramB, paramC));
}

// Choosing RNG
void Physics::select_RNG(unint rng_type)
{
	if (rng_type > 0) {
		this->rand_type = rng_type;
	}
	else {
		throw invalid_argument(
			"Error! RNG must be specified with positive integers.");
		exit(1);
	}
}

// Returns random double number.
double Physics::rand_double(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	switch (this->rand_type) {
	case 1:
		return (pow(-1,rand()%2))*this->uniform(0.0, (paramA+paramB)/2);
	case 2:
		return this->gaussian(paramA, paramB, paramC);
	case 3:
		return this->beta(paramA, paramB, paramC);
	case 4:
		return this->chi_square(paramA);
	case 5:
		return (double) this->binomial(
			(unsigned int)paramA, (unsigned int)paramB);
	default:
		return this->uniform(0, 1);
	}
}

// Updates current status
void Physics::update_status(
	double x, double y, double vx, double vy, \
	bool refl, double curr_time)
{
	this->x_loc.push_back(x);
	this->y_loc.push_back(y);
	this->x_vel.push_back(vx);
	this->y_vel.push_back(vy);

	this->curr_object->set_location(x, y);
	this->curr_object->set_velocity(vx, vy);

	this->reflected_status.push_back(refl);
	this->time_trace.push_back(curr_time);

	if (refl == true && this->b_verbose == true)
		this->print_status_rect();
}

// Log status into Physics
void Physics::log_status()
{
	this->x_loc.push_back(
		this->curr_object->x());
	this->y_loc.push_back(
		this->curr_object->y());

	this->x_vel.push_back(
		this->curr_object->xv());
	this->y_vel.push_back(
		this->curr_object->yv());

	this->reflected_status.push_back(false);
	this->time_trace.push_back(this->time_elapsed);
}

// Returns projected location after a certain time.
std_vec_d Physics::proj_loc_rect(double time_segment)
{
	std_vec_d proj_loc(2);

	proj_loc[0] = \
		this->curr_object->x()+this->curr_object->xv()*time_segment;
	proj_loc[1] = \
		this->curr_object->y()+this->curr_object->yv()*time_segment;

	return proj_loc;
}

// Returns current status.
std_vec_d Physics::report_status_rect()
{
	std_vec_d report_info(7);

	report_info[0] = this->curr_object->x();
	report_info[1] = this->curr_object->y();
	report_info[2] = this->curr_object->xv();
	report_info[3] = this->curr_object->yv();
	report_info[4] = this->curr_object->read_mass();
	report_info[5] = this->time_elapsed;
	if (this->reflected_status.back() == true) {
		report_info[6] = 1;
	}
	else {
		report_info[6] = 0;
	}

	return report_info;
}


// Reporting status to the stdio on the way.
void Physics::print_status_rect()
{
	cout << this->sprint_status_rect(std_str("\n")).c_str();
}

// Report status with string.
std_str Physics::sprint_status_rect(std_str linbreak = "\n")
{
	std_str report_str("");
	if (this->reflected_status.back() == true) {
		report_str = report_str \
			+ linbreak + "** Reflected!! **" \
			+ linbreak;
	}

	std_vec_d info(7);
	info = this->report_status_rect();
	report_str = report_str + \
		"Location: (" + this->double_to_string(info[0]) + "," \
		+ this->double_to_string(info[1]) + ")" + linbreak;
	report_str = report_str + \
		"Velocity: <" + this->double_to_string(info[2]) + "," \
		+ this->double_to_string(info[3]) + ">" + linbreak;
	report_str = report_str + \
		"Time elapsed: " + this->double_to_string(info[5]) + " sec."
		+ linbreak + linbreak;

	return report_str;
}

// Convert double to std::string object
std_str Physics::double_to_string(double input)
{
	o_sstream out_sstream;
	if (!(out_sstream << input)) {
		throw invalid_argument(
			"Bad Double to String stream Conversion!!");
		exit(1);
	}

	return out_sstream.str();
}

// Displaying dimension
std_str Physics::show_dimension_rect(std_str linbreak)
{
	std_str rect_report;
	rect_report = linbreak \
		+ "Rectangular dimnesion information" + linbreak \
		+ "Left: " + this->double_to_string(this->edge_left) + linbreak \
		+ "Right: " + this->double_to_string(this->edge_right) + linbreak \
		+ "Top: " + this->double_to_string(this->edge_top) + linbreak \
		+ "Bottom: " + this->double_to_string(this->edge_bottom) + linbreak \
		+ linbreak + linbreak;

	if (this->b_verbose == true) {
		cout << endl;
		cout << "Rectangular dimnesion information" << endl;
		cout << "Left: " << this->edge_left << endl;
		cout << "Rigth: " << this->edge_right << endl;
		cout << "Top: " << this->edge_top << endl;
		cout << "Bottom: " << this->edge_bottom << endl;
		cout << endl;
	}

	return rect_report;
}

// Set misc parameters
bool Physics::set_parameters(bool verbose, unint RNG_type)
{
	this->b_verbose = verbose;
	this->rand_type = RNG_type;
	return true;
}

bool Physics::set_timing(
	double d_time_limit, double d_time_scale)
{
	this->time_limit = d_time_limit;
	this->time_scale = d_time_scale;

	return true;
}

bool Physics::set_Molecule(Molecule* Thing)
{
	this->curr_object = Thing;

	return true;
}


bool Physics::set_dimension_rect(
	double dim_left, double dim_right, \
	double dim_top, double dim_bottom)
{
	if (dim_left > dim_right) { 
		throw invalid_argument(
			"Error, dimension left should be lower than right");
		return false;
	}
	else if (dim_top < dim_bottom) {
		throw invalid_argument(
			"Error, dimension bottom should be lower than top");
		return false;
	}
	else {
		this->edge_left = dim_left;
		this->edge_right = dim_right;
		this->edge_top = dim_top;
		this->edge_bottom = dim_bottom;
		return true;
	}
}


// Logging the trace within a specified file.
void Physics::write_log_rect(
	std_str outfile_name, std_str cDelim = "\t", std_str linbreak = "\n")
{
	ofstream trace_record;
	trace_record.open(outfile_name.c_str());
	trace_record << "\"Coord X\"" << cDelim << "\"Coord Y\"" \
		<< cDelim << "\"Velocity X\"" << cDelim << "\"Velocity Y\"" \
		<< cDelim << "\"Mass (g)\"" << cDelim << "\"Reflected\"" \
		<< cDelim << "\"Time\"" \
		<< linbreak;
	trace_record << scientific;

	unint log_size = this->time_trace.size();

	for (unint i=0; i < log_size; i++) {
		trace_record << this->x_loc.at(i) << cDelim << \
			this->y_loc.at(i) << cDelim << \
			this->x_vel.at(i) << cDelim << \
			this->y_vel.at(i) << cDelim << \
			this->curr_object->read_mass() << cDelim << \
			this->bool_to_yesno(this->reflected_status.at(i)) << cDelim << \
			this->time_trace.at(i) << \
			linbreak;
	}

	trace_record.close();
	cout << "File saved as ... " << outfile_name << endl;
}

// Return history with string.
std_str Physics::extract_log_rect(
	std_str cDelim = "\t", std_str linbreak = "\n")
{
	ostringstream trace_record;
	trace_record << "\"Coord X\"" << cDelim << "\"Coord Y\"" \
		<< cDelim << "\"Velocity X\"" << cDelim << "\"Velocity Y\"" \
		<< cDelim << "\"Mass (g)\"" << cDelim << "\"Reflected\"" \
		<< cDelim << "\"Time\"" \
		<< linbreak;

	unint log_size = this->time_trace.size();

	for (unint i = 0; i < log_size; i++) {
		trace_record << this->double_to_string(this->x_loc.at(i)) << cDelim << \
			this->double_to_string(this->y_loc.at(i)) << cDelim << \
			this->double_to_string(this->x_vel.at(i)) << cDelim << \
			this->double_to_string(this->y_vel.at(i)) << cDelim << \
			this->double_to_string(this->curr_object->read_mass()) << cDelim << \
			this->bool_to_yesno(this->reflected_status.at(i)) << cDelim << \
			this->double_to_string(this->time_trace.at(i)) << \
			linbreak;
	}

	return trace_record.str();
}

// A little tool to convert bool type to understandable string.
std_str Physics::bool_to_yesno(bool logic)
{
	if (logic == true)
		return "YES";
	else 
		return "NO";
}

// Calculate reflecting molecule in a limited space.
void Physics::reflect_rect(double time_frame)
{
	if (this->x_loc.back() > this->edge_right || \
		this->x_loc.back() < this->edge_left || \
		this->y_loc.back() > this->edge_top || \
		this->y_loc.back() < this->edge_bottom) {	
		throw invalid_argument(
			"The trace already found at the outside of \
			the boundary. Cannot proceed!!\n\n");
		exit(1);
	}

	double delta_t = time_frame*1e-4;
	double div_time = 0.0;
	std_vec_d adv_loc(2);
	
	do {
		adv_loc = this->proj_loc_rect(delta_t);		
		
		if (adv_loc[0] <= edge_left) {
			this->time_elapsed += delta_t;
			this->update_status(this->edge_left, adv_loc[1], \
				(-1)*this->curr_object->xv(), this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[0] >= edge_right) {
			this->time_elapsed += delta_t;
			this->update_status(this->edge_right, adv_loc[1], \
				(-1)*this->curr_object->xv(), this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[1] <= edge_bottom) {
			this->time_elapsed += delta_t;
			this->update_status(adv_loc[0], this->edge_bottom, \
				this->curr_object->xv(), (-1)*this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[1] >= edge_top) {
			this->time_elapsed += delta_t;
			this->update_status(adv_loc[0], this->edge_top, \
				this->curr_object->xv(), (-1)*this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[0] > this->edge_left && \
			adv_loc[0] < this->edge_right && \
			adv_loc[1] > this->edge_bottom && \
			adv_loc[1] < this->edge_top) {
			
			this->time_elapsed += delta_t;
			this->curr_object->set_location(adv_loc[0], adv_loc[1]);
		}

		div_time += delta_t;

	} while (div_time <= time_frame);
}






//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

Physics::Physics(Molecule* Thing)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{	
	this->Seed_Rand();

	this->time_elapsed = 0.0;
	this->time_scale = 1.0; // Default 1 sec time interval.
	this->curr_object = Thing;
	this->rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = false;
}

Physics::Physics(Molecule* Thing, bool verbose)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	this->Seed_Rand();

	this->time_elapsed = 0.0;
	this->time_scale = 1.0; // Default 1 sec time interval.
	this->curr_object = Thing;
	this->rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = verbose;
}


Physics::Physics(
	Molecule* Thing, double time_limit, double time_sc)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	this->Seed_Rand();

	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;
	this->rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = false;
}

Physics::Physics(
	Molecule* Thing, \
	double time_limit, double time_sc, \
	bool verbose)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	this->Seed_Rand();

	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;
	this->rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = verbose;
}

Physics::Physics(
	Molecule* Thing, \
	double time_limit, double time_sc, \
	bool verbose, \
	unint rand_type = 1)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	this->Seed_Rand();
	
	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;
	this->rand_type = rand_type;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = verbose;
}

Physics::Physics(
	Molecule* Thing,\
	double time_limit, double time_sc,\
	double rect_left, double rect_right,\
	double rect_top, double rect_bottom,\
	bool verbose,\
	unint rand_type = 1)
{
	this->Seed_Rand();
	
	if (this->set_dimension_rect(
		rect_left, rect_right, rect_top, rect_bottom) != true) {
		throw invalid_argument(
			"Un oh... wrong dimension detected!!");
	}

	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;
	this->rand_type = rand_type;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = verbose;
}

Physics::~Physics()
{
	this->x_loc.clear();
	this->y_loc.clear();
	this->x_vel.clear();
	this->y_vel.clear();
	this->curr_object = NULL;

	this->time_trace.clear();

	this->reflected_status.clear();
}

#endif
