#ifndef PHYSICS_CPP
#define PHYSICS_CPP

#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include "Physics.hpp"

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

	this->curr_object->set_location( \
		this->x_loc.back(),this->y_loc.back());
	this->curr_object->set_velocity( \
		this->x_vel.back(),this->y_vel.back());

	this->time_elapsed += this->time_scale;
	this->time_trace.push_back(this->time_elapsed);
	this->reflected_status.push_back(false);

	this->print_status_rect();
}

void Physics::brownian_rect(\
	float max_vel_x, float max_vel_y, \
	float edge_left, float edge_right, \
	float edge_top, float edge_bottom)
{
	// Display the system
	cout << endl;
	cout << "Edge information" << endl;
	cout << "Left: " << edge_left << endl;
	cout << "Rigth: " << edge_right << endl;
	cout << "Top: " << edge_top << endl;
	cout << "Bottom: " << edge_bottom << endl;
	cout << endl;

	std_vec_f proj_loc(2);

	srand((unsigned int) time(0));

	// Initializing the first point with a random velocity vectors.
	this->curr_object->set_velocity(\
		this->rand_float(0,max_vel_x), \
		this->rand_float(0,max_vel_y));

	// Log status into the class.
	this->log_status();

	do {
		// Running reflectance for this vector.
		proj_loc = proj_loc_rect(this->time_scale);

		if (proj_loc[0] < edge_left || proj_loc[0] > edge_right \
		|| proj_loc[1] < edge_bottom || proj_loc[1] > edge_top) {
			this->reflect_rect(edge_left, edge_right, edge_top, edge_bottom);
			this->curr_object->set_velocity(\
				this->rand_float(0,max_vel_x), \
				this->rand_float(0,max_vel_y));
			this->log_status();
		}
		else {
			this->time_elapsed += this->time_scale;
			this->curr_object->set_location(proj_loc[0], proj_loc[1]);
			this->curr_object->set_velocity(\
				this->rand_float(0,max_vel_x), \
				this->rand_float(0,max_vel_y));
			this->log_status();
		}

			
	} while (this->time_elapsed < this->time_limit);
}

// Updates current status
void Physics::update_status(\
	float x, float y, float vx, float vy, \
	bool refl, float curr_time)
{
	this->x_loc.push_back(x);
	this->y_loc.push_back(y);
	this->x_vel.push_back(vx);
	this->y_vel.push_back(vy);

	this->curr_object->set_location(x, y);
	this->curr_object->set_velocity(vx, vy);

	this->reflected_status.push_back(refl);
	this->time_trace.push_back(curr_time);

	this->print_status_rect();
}

// Log status into Physics
void Physics::log_status()
{
	this->x_loc.push_back(\
		this->curr_object->x());
	this->y_loc.push_back(\
		this->curr_object->y());

	this->x_vel.push_back(\
		this->curr_object->xv());
	this->y_vel.push_back(\
		this->curr_object->yv());

	this->reflected_status.push_back(false);
	this->time_trace.push_back(this->time_elapsed);
}

// Detects the initial collision location and returns it as float, float vector.
std_vec_f Physics::collision_rect(\
	float edge_left, float edge_right, \
	float edge_top, float edge_bottom, \
	float time_segment)
{
	std_vec_f col_loc(2);
	std_vec_f proj_loc(2);
	std_vec_f curr_loc(2);
	std_vec_f temp_loc(2);
	float temp_time = 0.0;
	float temp_time_sc = this->time_scale * 1E-5;

	proj_loc = this->proj_loc_rect(time_segment);
	curr_loc[0] = this->x_loc.back();
	curr_loc[1] = this->y_loc.back();

	temp_loc = curr_loc;
	int i = 0;

	do {
		temp_loc = this->proj_loc_rect(temp_time);
		temp_time = temp_time_sc*i;
		
		if (temp_loc[0] > edge_right) {
			temp_loc[0] = edge_right;
			break;
		}
		else if (temp_loc[0] < edge_left) {
			temp_loc[0] = edge_left;
			break;
		}
		else if (temp_loc[1] > edge_top) {
			temp_loc[1] = edge_top;
			break;
		}
		else if (temp_loc[1] < edge_bottom) {
			temp_loc[1] = edge_bottom;
			break;
		}

		i++;

	} while (1);

	col_loc = temp_loc;

	return col_loc;
}

// Returns projected location after a certain time.
std_vec_f Physics::proj_loc_rect(float time_segment)
{
	std_vec_f proj_loc(2);

	proj_loc[0] = \
		this->curr_object->x()+this->curr_object->xv()*time_segment;
	proj_loc[1] = \
		this->curr_object->y()+this->curr_object->yv()*time_segment;

	return proj_loc;	
}

// Reporting status on the way.
void Physics::print_status_rect()
{
	if (this->reflected_status.back() == true) {
		cout << "** Reflected!! **" << endl;
	}

	cout << "Location: (" << this->x_loc.back() << \
		 "," << this->y_loc.back() << ")" << endl;
	cout << "Velocity: <" << this->x_vel.back() << \
		 "," << this->y_vel.back() << ">" << endl;
	cout << "Time elapsed: " << this->time_elapsed \
		<< " sec." << endl;
	cout << endl;
}

// Prepare log file.
void Physics::write_log_rect(std_str outfile_name)
{
	ofstream trace_record;
	trace_record.open(outfile_name);
	trace_record << "\"Coord X\"" << "\t" << "\"Coord Y\"" \
		<< "\t" << "\"Velocity X\"" << "\t" << "\"Velocity Y\"" \
		<< "\t" << "\"Mass (g)\"" << "\t" << "\"Reflected\"" \
		<< "\t" << "\"Time\"" \
		<< endl;
	trace_record << scientific;

	unsigned long log_size = this->time_trace.size();

	for (int i=0; i < log_size; i++) {
		trace_record << this->x_loc.at(i) << "\t" << \
			this->y_loc.at(i) << "\t" << \
			this->x_vel.at(i) << "\t" << \
			this->y_vel.at(i) << "\t" << \
			this->curr_object->mass << "\t" << \
			this->bool_to_yesno(this->reflected_status.at(i)) << "\t" << \
			this->time_trace.at(i) << \
			endl;
	}

	trace_record.close();
	cout << "File saved as ... " << outfile_name << endl;
}

// A little tool to convert bool type to understandable string.
std_str Physics::bool_to_yesno(bool logic)
{
	if (logic == true) {
		return "YES";
	}
	else {
		return "NO";
	}
}

// Returns random float number.
float Physics::rand_float(float min, float max)
{
	return pow(-1,rand()%2)*((max-min)*((float)rand()/RAND_MAX))+min;
}

// Calculate reflecting molecule in a limited space.
void Physics::reflect_rect(float edge_left, float edge_right, \
		float edge_top, float edge_bottom)
{
	if (this->x_loc.back() > edge_right || \
		this->x_loc.back() < edge_left || \
		this->y_loc.back() > edge_top || \
		this->y_loc.back() < edge_bottom) {	
		throw invalid_argument( \
			"The trace already found at the outside of \
			the boundary. Cannot proceed!!\n\n");
		exit(1);		
	}

	float delta_t = this->time_scale*1e-5;
	float div_time = 0.0;
	std_vec_f adv_loc(2);
	
	do {
		adv_loc = this->proj_loc_rect(delta_t);		
		
		if (adv_loc[0] <= edge_left) {
			this->time_elapsed += delta_t;
			this->update_status(edge_left, adv_loc[1], \
				(-1)*this->curr_object->xv(), this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[0] >= edge_right) {
			this->time_elapsed += delta_t;
			this->update_status(edge_right, adv_loc[1], \
				(-1)*this->curr_object->xv(), this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[1] <= edge_bottom) {
			this->time_elapsed += delta_t;
			this->update_status(adv_loc[0], edge_bottom, \
				this->curr_object->xv(), (-1)*this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[1] >= edge_top) {
			this->time_elapsed += delta_t;
			this->update_status(adv_loc[0], edge_top, \
				this->curr_object->xv(), (-1)*this->curr_object->yv(), \
				true, this->time_elapsed);
		}
		if (adv_loc[0] > edge_left && \
			adv_loc[0] < edge_right && \
			adv_loc[1] > edge_bottom && \
			adv_loc[1] < edge_top) {
			//cout << "Continuing to next delta_t" << endl;
			this->time_elapsed += delta_t;
			this->curr_object->set_location(adv_loc[0], adv_loc[1]);
			//this->curr_object->set_velocity(this->x_vel.back(),this->y_vel.back());
		}

		div_time += delta_t;

	} while (div_time <= this->time_scale);
}



// Constructors and Destructors

Physics::Physics()
{
	this->time_elapsed = 0.0;
	this->time_scale = 1.0;
	this->curr_object = NULL;

	this->obj_mass = 0.0;

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);
}

Physics::Physics(Molecule* Thing)
{
	this->time_elapsed = 0.0;
	this->time_scale = 1.0; // Default 1 sec time interval.
	this->curr_object = Thing;

	vector<float> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);
}

Physics::Physics(float time_limit, float time_sc, Molecule* Thing)
{
	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;

	vector<float> obj_info(5);
	obj_info = Thing->read_info();
	this->x_loc.push_back(obj_info[0]);
	this->y_loc.push_back(obj_info[1]);
	this->x_vel.push_back(obj_info[2]);
	this->y_vel.push_back(obj_info[3]);
	this->obj_mass = obj_info[4];

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);
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