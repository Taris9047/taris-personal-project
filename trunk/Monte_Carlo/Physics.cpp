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

void Physics::brownian_rect(\
	double max_vel_x, double max_vel_y, \
	double edge_left, double edge_right, \
	double edge_top, double edge_bottom)
{
	// Display the system
	cout << endl;
	cout << "Edge information" << endl;
	cout << "Left: " << edge_left << endl;
	cout << "Rigth: " << edge_right << endl;
	cout << "Top: " << edge_top << endl;
	cout << "Bottom: " << edge_bottom << endl;
	cout << endl;

	std_vec_d proj_loc(2);
	double time_frame;

	// Initializing the first point with a random velocity vectors.
	/*
	this->curr_object->set_velocity(\
		this->rand_gen->uniform((-1)*max_vel_x,max_vel_x), \
		this->rand_gen->uniform((-1)*max_vel_y,max_vel_y));
	*/
	this->curr_object->set_velocity(\
		this->gaussian(0, max_vel_x/4, max_vel_x), \
		this->gaussian(0, max_vel_y/4, max_vel_y));

	// Log status into the class.
	this->log_status();

	do {
		// Running reflectance for this vector.
		if ( (this->time_elapsed + this->time_scale) > this->time_limit ) {
        	time_frame = this->time_limit - this->time_elapsed;
        	if ( time_frame == 0 ) {
        		break;
        	}
        }
        else {
        	time_frame = this->time_scale;
        }
        
		proj_loc = proj_loc_rect(time_frame);

		if (proj_loc[0] < edge_left || proj_loc[0] > edge_right \
		|| proj_loc[1] < edge_bottom || proj_loc[1] > edge_top) {
			this->reflect_rect( \
                edge_left, edge_right, edge_top, edge_bottom, \
                time_frame);
		}
		else {
            this->curr_object->set_location(proj_loc[0], proj_loc[1]);
            this->time_elapsed += time_frame;
		}

		if (this->b_verbose == true) {
			this->print_status_rect();
		}
		/*
		this->curr_object->set_velocity(\
			this->rand_gen->uniform((-1)*max_vel_x,max_vel_x), \
			this->rand_gen->uniform((-1)*max_vel_y,max_vel_y));
		*/
		this->curr_object->set_velocity(\
			this->gaussian(0, max_vel_x/4, max_vel_x), \
			this->gaussian(0, max_vel_y/4, max_vel_y));
		
		this->log_status();

	} while (this->time_elapsed <= this->time_limit);
}

// Updates current status
void Physics::update_status(\
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

	if (this->b_verbose == true) {
		this->print_status_rect();
	}
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
	report_info[4] = this->curr_object->mass;
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
	if (this->reflected_status.back() == true) {
		cout << "** Reflected!! **" << endl;
	}

	std_vec_d info(7);

	info = this->report_status_rect();

	cout << "Location: (" << info[0] << \
		 "," << info[1] << ")" << endl;
	cout << "Velocity: <" << info[2] << \
		 "," << info[3] << ">" << endl;
	cout << "Time elapsed: " << info[5] \
		<< " sec." << endl;
	cout << endl;


/*
	cout << "Location: (" << this->x_loc.back() << \
		 "," << this->y_loc.back() << ")" << endl;
	cout << "Velocity: <" << this->x_vel.back() << \
		 "," << this->y_vel.back() << ">" << endl;
	cout << "Time elapsed: " << this->time_elapsed \
		<< " sec." << endl;
	cout << endl;
*/
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

// Returns random double number.
//double Physics::rand_double(double min, double max)
//{
//	return pow(-1,rand()%2)*((max-min)*((double)rand()/RAND_MAX))+min;
//}

// Calculate reflecting molecule in a limited space.
void Physics::reflect_rect(double edge_left, double edge_right, \
		double edge_top, double edge_bottom, double time_frame)
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

	double delta_t = time_frame*1e-4;
	double div_time = 0.0;
	std_vec_d adv_loc(2);
	
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
			
			this->time_elapsed += delta_t;
			this->curr_object->set_location(adv_loc[0], adv_loc[1]);
		}

		div_time += delta_t;

	} while (div_time <= time_frame);
}






//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

Physics::Physics()
{
	this->time_elapsed = 0.0;
	this->time_scale = 1.0;
	this->curr_object = NULL;
	
	this->obj_mass = 0.0;

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = false;
}

Physics::Physics(bool verbose)
{
	this->time_elapsed = 0.0;
	this->time_scale = 1.0;
	this->curr_object = NULL;

	this->obj_mass = 0.0;

	this->time_trace.push_back(this->time_elapsed);

	this->reflected_status.push_back(false);

	this->b_verbose = verbose;
}

Physics::Physics(Molecule* Thing)
{
	this->time_elapsed = 0.0;
	this->time_scale = 1.0; // Default 1 sec time interval.
	this->curr_object = Thing;

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
{
	this->time_elapsed = 0.0;
	this->time_scale = 1.0; // Default 1 sec time interval.
	this->curr_object = Thing;

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


Physics::Physics(double time_limit, double time_sc, Molecule* Thing)
{
	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;

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

Physics::Physics(double time_limit, double time_sc, Molecule* Thing, bool verbose)
{
	this->time_elapsed = 0.;
	this->time_limit = time_limit;
	this->time_scale = time_sc;
	this->curr_object = Thing;

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
