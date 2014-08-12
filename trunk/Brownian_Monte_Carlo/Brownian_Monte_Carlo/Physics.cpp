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

void Physics::brownian_rect(float max_vel_x, float max_vel_y, \
	float edge_left, float edge_right, \
	float edge_top, float edge_bottom)
{
	srand(time(0));

	do {
		if (this->x_vel.back() == 0.0 && this->y_vel.back() == 0.0) {
			this->x_vel.pop_back();
			this->x_vel.push_back((float)rand()/max_vel_x);
			this->y_vel.pop_back();
			this->y_vel.push_back((float)rand()/max_vel_y);		
		}

		// Display the system
		cout << endl;
		cout << "Edge information" << endl;
		cout << "Left: " << edge_left << endl;
		cout << "Rigth: " << edge_right << endl;
		cout << "Top: " << edge_top << endl;
		cout << "Bottom: " << edge_bottom << endl;
		cout << endl;

		// Displaying current state.
		this->print_status_rect();

		// Running reflectance for this vector.
		this->reflect_rect(edge_left, edge_right, edge_top, edge_bottom);

		this->x_loc.push_back(this->x_loc.back()+\
			this->time_scale*this->x_vel.back());
		this->y_loc.push_back(this->y_loc.back()+\
			this->time_scale*this->y_vel.back());
		this->x_vel.push_back((float)rand()/max_vel_x);
		this->y_vel.push_back((float)rand()/max_vel_y);		

		this->curr_object->set_location( \
			this->x_loc.back(),this->y_loc.back());
		this->curr_object->set_velocity( \
			this->x_vel.back(),this->y_vel.back());

		this->time_elapsed += this->time_scale;	
		this->time_trace.push_back(this->time_elapsed);
		this->reflected_status.push_back(false);

	} while (this->time_elapsed < this->time_limit);
}

// Detects the initial collision location and returns it as float, float vector.
std_vec_f Physics::collision_rect(float edge_left, float edge_right, \
		float edge_top, float edge_bottom, float time_segment)
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
		this->x_loc.back()+this->x_vel.back()*time_segment;
	proj_loc[1] = \
		this->y_loc.back()+this->y_vel.back()*time_segment;

	return proj_loc;
}

// Reporting status on the way.
void Physics::print_status_rect()
{
	std_vec_f curr_loc(2);
	std_vec_f curr_vel(2);

	curr_loc[0] = this->x_loc.back();
	curr_loc[1] = this->y_loc.back();
	curr_vel[0] = this->x_vel.back();
	curr_vel[1] = this->y_vel.back();

	if (this->reflected_status.back() == true) {
		cout << "** Reflected!! **" << endl;
	}

	cout << "Location: (" << curr_loc[0] << \
		 "," << curr_loc[1] << ")" << endl;
	cout << "Velocity: <" << curr_vel[0] << \
		 "," << curr_vel[1] << ">" << endl;
	cout << "Time elapsed: " << this->time_elapsed \
		<< " sec." << endl;

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

	unsigned int log_size = this->time_trace.size();

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

	float div_time = 0.0; // Must be smaller than time scale.
	float div_time_left = this->time_scale - div_time;
	std_vec_f proj_loc(2);
	float time_elapsed_ref = 0.0;

	do {
		proj_loc = this->proj_loc_rect(div_time_left);

		if ( edge_left < proj_loc[0] && proj_loc[0] < edge_right \
			&& edge_bottom < proj_loc[1] && proj_loc[1] < edge_top) {
			// Projected location is inside the system. Thus, skip
			// this crap!
			cout << "reflection estimation is not necessary." << endl;
			break;
		}

		float next_x = proj_loc[0];
		float next_y = proj_loc[1];

		std_vec_f collision_loc(2);
		collision_loc = this->collision_rect( \
			edge_left, edge_right, edge_top, edge_bottom, \
			div_time_left);

		if (collision_loc[0] == edge_left || \
			collision_loc[0] == edge_right) {
			
			float offset_x = 0.0;
			float next_y_ref = this->y_vel.back(); 
			float next_x_ref = this->x_vel.back(); 
			
			// Calculating how much time elapsed before 
			// reflection from x direction.
			if (collision_loc[0] > edge_right) {
				offset_x = next_x - edge_right;
				time_elapsed_ref = offset_x/this->x_vel.back();
				next_x_ref = edge_right;
			}
			else if (collision_loc[0] < edge_left) {
				offset_x = edge_left - next_x;
				time_elapsed_ref = abs(offset_x)/this->x_vel.back();
				next_x_ref = edge_left;
			}

			// Update y corresponding to x until hitting
			// the wall.
			next_y_ref = this->y_loc.back()+\
				this->y_vel.back()*time_elapsed_ref;

			// Update status to Molecule class.
			this->x_loc.push_back(next_x_ref);
			this->y_loc.push_back(next_y_ref);
			this->x_vel.push_back(this->x_vel.back()*(-1));
			this->y_vel.push_back(this->y_vel.back());
			this->time_elapsed += time_elapsed_ref;
			this->time_trace.push_back(this->time_elapsed);
			this->reflected_status.push_back(true);
			div_time += time_elapsed_ref;
			div_time_left = this->time_scale - div_time;

		}

		else if (collision_loc[1] == edge_top || \
			collision_loc[1] == edge_bottom) {
			float offset_y = 0.0;
			float next_y_ref = this->y_vel.back(); 
			float next_x_ref = this->x_vel.back(); 
			
			// Calculating how much time elapsed before 
			// reflection from x direction.
			if (collision_loc[1] > edge_top) {
				offset_y = next_x - edge_top;
				time_elapsed_ref = offset_y/this->y_vel.back();
				next_y_ref = edge_top;
			}
			else if (collision_loc[1] < edge_bottom) {
				offset_y = edge_bottom - next_y;
				time_elapsed_ref = abs(offset_y)/this->y_vel.back();
				next_y_ref = edge_bottom;
			}

			// Update y corresponding to x until hitting
			// the wall.
			next_x_ref = this->x_loc.back()+\
				this->x_vel.back()*time_elapsed_ref;

			// Update status to Molecule class.
			this->x_loc.push_back(next_x_ref);
			this->y_loc.push_back(next_y_ref);
			this->x_vel.push_back(this->x_vel.back()*(-1));
			this->y_vel.push_back(this->y_vel.back());
			this->time_elapsed += time_elapsed_ref;
			this->time_trace.push_back(this->time_elapsed);
			this->reflected_status.push_back(true);
			div_time += time_elapsed_ref;
			div_time_left = this->time_scale - div_time;
		}

		// When velocity vector head directly to the very tips.
		// 
		else if ( (collision_loc[0] == edge_left && \
			collision_loc[1] == edge_bottom) || \
			(collision_loc[0] == edge_left && \
			collision_loc[1] == edge_top) || \
			(collision_loc[0] == edge_right && \
			collision_loc[1] == edge_bottom) || \
			(collision_loc[0] == edge_right && \
			collision_loc[1] == edge_top) ) {

			float offset_x = 0.0;
			float next_y_ref = this->y_vel.back(); 
			float next_x_ref = this->x_vel.back(); 
			
			// Calculating how much time elapsed before 
			// reflection from x direction.
			if (collision_loc[0] > edge_right) {
				offset_x = next_x - edge_right;
				time_elapsed_ref = offset_x/this->x_vel.back();
				next_x_ref = edge_right;
			}
			else if (collision_loc[0] < edge_left) {
				offset_x = edge_left - next_x;
				time_elapsed_ref = abs(offset_x)/this->x_vel.back();
				next_x_ref = edge_left;
			}

			// Update y corresponding to x until hitting
			// the wall.
			next_y_ref = this->y_loc.back()+\
				this->y_vel.back()*time_elapsed_ref;

			// Update status to Molecule class.
			this->x_loc.push_back(next_x_ref);
			this->y_loc.push_back(next_y_ref);
			this->x_vel.push_back(this->x_vel.back()*(-1));
			this->y_vel.push_back(this->y_vel.back()*(-1));
			this->time_elapsed += time_elapsed_ref;
			this->time_trace.push_back(this->time_elapsed);
			this->reflected_status.push_back(true);
			div_time += time_elapsed_ref;
			div_time_left = this->time_scale - div_time;	
		}

		this->print_status_rect();
		cout << div_time << endl;

	} while (div_time < this->time_scale);
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