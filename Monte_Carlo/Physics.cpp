#ifndef PHYSICS_CPP
#define PHYSICS_CPP

#include "Physics.hpp"
#include "Random.hpp"

using namespace std;

//////////////////////////////
//         Methods          //
//////////////////////////////
void Physics::advance_time()
{
	x_loc.push_back(x_loc.back()+\
		time_scale*x_vel.back());
	y_loc.push_back(y_loc.back()+\
		time_scale*y_vel.back());

	x_vel.push_back(0.0);
	y_vel.push_back(0.0);

	curr_object->set_location( \
		x_loc.back(),y_loc.back());
	curr_object->set_velocity( \
		x_vel.back(),y_vel.back());

	time_elapsed += time_scale;
	time_trace.push_back(time_elapsed);
	reflected_status.push_back(false);

	if (b_verbose == true) {
		print_status_rect();
	}
}

// Normal movement
void Physics::normal_rect(double vel_x, double vel_y)
{
	double time_frame = time_scale;
	std_vec_d proj_loc(2);

	if (b_verbose) {
		show_dimension_rect("\n");
		show_siminfo_rect("\n", false);
		cout << "Initial Condition:" << endl;
		print_status_rect();
	}

	curr_object->set_velocity(vel_x, vel_y);
	log_status();

	do {
		if ( (time_elapsed + time_scale) > time_limit ) {
			time_frame = time_limit - time_elapsed;
			if (time_frame == 0) break;
		}

		proj_loc = proj_loc_rect(time_frame);

		if (proj_loc[0] < edge_left || proj_loc[0] > edge_right \
			|| proj_loc[1] < edge_bottom || proj_loc[1] > edge_top)
			reflect_rect(time_frame);
		else {
            curr_object->set_location(proj_loc[0], proj_loc[1]);
            time_elapsed += time_frame;
		}

		if (b_verbose) print_status_rect();

		log_status();

	} while (time_elapsed <= time_limit);
}

// Simulating brownian movement
void Physics::brownian_rect(double max_vel_x, double max_vel_y)
{
	std_vec_d proj_loc(2);
	double time_frame = time_scale;

	if (b_verbose) {
		show_dimension_rect("\n");
		show_siminfo_rect("\n");
		cout << "Initial Condition:" << endl;
		print_status_rect();
	}
	// Initializing the first point with a random velocity vectors.
	set_rand_velocity_rect(
		max_vel_x / 2, max_vel_y / 3, (max_vel_x+max_vel_y)/2);

	// Log status into the class.
	log_status();

	do {
		// Running reflectance for this vector.
		if ( (time_elapsed + time_scale) > time_limit ) {
		   	time_frame = time_limit - time_elapsed;
        	if ( time_frame == 0 ) break;
        }
         
		proj_loc = proj_loc_rect(time_frame);

		if (proj_loc[0] < edge_left || proj_loc[0] > edge_right \
			|| proj_loc[1] < edge_bottom || proj_loc[1] > edge_top)
			reflect_rect(time_frame);
		else {
            curr_object->set_location(proj_loc[0], proj_loc[1]);
            time_elapsed += time_frame;
		}

		if (b_verbose == true)
			print_status_rect();

		set_rand_velocity_rect(
			max_vel_x / 2, max_vel_y / 3, (max_vel_x+max_vel_y)/2);

		log_status();

	} while (time_elapsed <= time_limit);
}

// Setting random velocity
void Physics::set_rand_velocity_rect(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	curr_object->set_velocity(
		rand_double(paramA, paramB, paramC), \
		rand_double(paramA, paramB, paramC));
}

// Setting random velocity for each axis
void Physics::set_rand_velocity_rect_x(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	curr_object->set_xv(
		rand_double(paramA, paramB, paramC));
}

// Setting random velocity for each axis
void Physics::set_rand_velocity_rect_y(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	curr_object->set_yv(
		rand_double(paramA, paramB, paramC));
}

// Choosing RNG
void Physics::select_RNG(unint rng_type)
{
	rand_type = rng_type;
}

// Returns random double number.
double Physics::rand_double(
	double paramA = 0., double paramB = 1., double paramC = 1.)
{
	switch (rand_type) {
	case 1:
		return (pow(-1,rand()%2))*\
			uniform(0.0, (paramA+paramB)/2);
	case 2:
		return gaussian(paramA, paramB, paramC);
	case 3:
		return beta(paramA, paramB, paramC);
	case 4:
		return chi_square(paramA, paramC);
	case 5:
		return (double) binomial(
			(unsigned long long)paramA, \
			(unsigned long long)paramB, paramC);
	case 6:
		return (double) poisson(
			paramA, (ullong)paramB, paramC);
	default:
		return uniform(0, 1);
	}
}

// Updates current status
void Physics::update_status(
	double x, double y, double vx, double vy, \
	bool refl)
{
	curr_object->set_location(x, y);
	curr_object->set_velocity(vx, vy);
	
	x_loc.push_back(
		curr_object->x());
	y_loc.push_back(
		curr_object->y());

	x_vel.push_back(
		curr_object->xv());
	y_vel.push_back(
		curr_object->yv());

	reflected_status.push_back(refl);
	time_trace.push_back(time_elapsed);

	if (refl == true && b_verbose == true)
		print_status_rect();
}

// Log status into Physics
void Physics::log_status()
{
	x_loc.push_back(
		curr_object->x());
	y_loc.push_back(
		curr_object->y());

	x_vel.push_back(
		curr_object->xv());
	y_vel.push_back(
		curr_object->yv());

	reflected_status.push_back(false);
	time_trace.push_back(time_elapsed);
}

// Returns projected location after a certain time.
std_vec_d Physics::proj_loc_rect(double time_segment)
{
	std_vec_d proj_loc(2);

	proj_loc[0] = \
		curr_object->x()+curr_object->xv()*time_segment;
	proj_loc[1] = \
		curr_object->y()+curr_object->yv()*time_segment;

	return proj_loc;
}

// Returns current status.
std_vec_d Physics::report_status_rect()
{
	std_vec_d report_info(7);

	report_info[0] = curr_object->x();
	report_info[1] = curr_object->y();
	report_info[2] = curr_object->xv();
	report_info[3] = curr_object->yv();
	report_info[4] = curr_object->read_mass();
	report_info[5] = time_elapsed;

	if (reflected_status.back() == true) {
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
	//cout.flush();
	cout << sprint_status_rect(std_str("\n")).c_str();
}

// Report status with string.
std_str Physics::sprint_status_rect(std_str linbreak = "\n")
{
	std_str report_str("");

	if (reflected_status.back() == true)
		report_str = report_str + \
			linbreak + "** Reflected!! **" + linbreak;

	std_vec_d info(7);
	info = report_status_rect();
	report_str = report_str + \
		"Location: (" + Converters::numtostdstr(info[0]) + "," \
		+ Converters::numtostdstr(info[1]) + ")" + linbreak + \
		"Velocity: <" + Converters::numtostdstr(info[2]) + "," \
		+ Converters::numtostdstr(info[3]) + ">" + linbreak + \
		"Time elapsed: " + Converters::numtostdstr(info[5]) + " sec." + \
		linbreak;

	return report_str;
}

// Displaying dimension
std_str Physics::show_dimension_rect(std_str linbreak = "\n")
{
	std_str rect_report;
	rect_report = rect_report + \
		+ "** Rectangular dimnesion information **" + linbreak \
		+ "Left: " + Converters::numtostdstr(edge_left) + linbreak \
		+ "Right: " + Converters::numtostdstr(edge_right) + linbreak \
		+ "Top: " + Converters::numtostdstr(edge_top) + linbreak \
		+ "Bottom: " + Converters::numtostdstr(edge_bottom) + linbreak \
		+ linbreak;

	if (b_verbose == true)
		cout << rect_report;

	return rect_report;
}

// Displaying simulation info.
std_str Physics::show_siminfo_rect(
	std_str linbreak = "\n", bool use_RNG)
{
	std_str rect_sim_report;

	rect_sim_report = rect_sim_report + \
		+ "** Simulation information **" + linbreak \
		+ "Calculation time: " \
		+ Converters::numtostdstr(this->time_limit) \
		+ " seconds" + linbreak;

	if (use_RNG == true)
		rect_sim_report = rect_sim_report + \
			"RNG Type: " + get_RNG_type() + \
			linbreak;

	rect_sim_report = rect_sim_report + linbreak;

	if (b_verbose == true)
		cout << rect_sim_report;

	return rect_sim_report;
}

// Set misc parameters
bool Physics::set_parameters(
	bool verbose, unint RNG_type)
{
	b_verbose = verbose;

	if (RNG_type > 6 || RNG_type < 1) {
		throw "Using default RNG: Uniform distribution for (0,1)";
		rand_type = RNG_type;
	}
	else {
		rand_type = RNG_type;
	}

	return true;
}

bool Physics::set_timing(
	double d_time_limit, double d_time_scale)
{
	if (d_time_limit < 0. || d_time_scale < 0.) {
		throw invalid_argument(
			"Uh oh... it's not a time machine simulator!!\nPlease use positive values for time variables.");
		exit(1);
	}

	time_limit = d_time_limit;
	time_scale = d_time_scale;

	return true;
}

bool Physics::set_Molecule(Molecule* Thing)
{
	if (!(curr_object = Thing)) {
		throw "Physics::set_Molecule went haywire!!";
		return false;
	}
	return true;
}

bool Physics::set_Photon(Photon* pPhoton)
{
	if (!(curr_photon = pPhoton)) {
		throw "Physics::set_Photon went haywire!!";
		return false;
	}
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

// Force set location
void Physics::set_location_rect(double xloc, double yloc)
{
	if (xloc > edge_right || xloc < edge_left) {
		throw invalid_argument(
			"Wrong location for x. Out of the rectangular dimension!!");
		exit(1);
	}
	if (yloc > edge_top || yloc < edge_bottom) {
		throw invalid_argument(
			"Wrong location for y. Out of the rectangular dimension!!");
		exit(1);
	}

	this->curr_object->set_x(xloc);
	this->curr_object->set_y(yloc);
}

// Reset log
void Physics::Reset_Sim()
{
	x_loc.clear();
	y_loc.clear();
	x_vel.clear();
	y_vel.clear();

	time_trace.clear();
	reflected_status.clear();

	curr_object->set_x(0.0);
	curr_object->set_xv(0.0);
	curr_object->set_y(0.0);
	curr_object->set_yv(0.0);

	time_elapsed = 0.0;

	log_status();
}

// Sanity Check
void Physics::Validate_Data()
{
	if (time_limit < 0) {
		throw invalid_argument("Given time limit must be positive!!");
		exit(1);
	}

	if (time_scale < 0) {
		throw invalid_argument("Time scale must be positive!!");
		exit(1);
	}

	if (rand_type > 6 && rand_type < 1) {
		throw "Using default RNG. Uniform distribution for (0,1)";
	}

	if (edge_left > edge_right) {
		throw invalid_argument("Wring rectangular assignment!! Left should be smaller than Right.");
		exit(1);
	}

	if (edge_top < edge_bottom) {
		throw invalid_argument("Wring rectangular assignment!! Bottom should be smaller than Top.");
		exit(1);
	}
}

// Return Random Number Generator type as std::string
std_str Physics::get_RNG_type()
{
	switch (rand_type) {
	case 1:
		return "Uniform Distribution";
	case 2:
		return "Gaussian Distribution";
	case 3:
		return "Beta Distribution";
	case 4:
		return "Chi-Square Distribution";
	case 5:
		return "Binomial Distribution";
	case 6:
		return "Poisson Distribution";
	default:
		return "Default Uniform Distrubition (0 to 1)";
	}
}

// Logging the trace within a specified file.
void Physics::write_log_rect(
	std_str outfile_name, \
	std_str cDelim = "\t", std_str linbreak = "\n")
{
	ofstream trace_record;
	trace_record.open(outfile_name.c_str());
	
	trace_record << extract_log_rect(cDelim, linbreak);

	trace_record.close();
	cout << "File saved as ... " << outfile_name << endl;
}

// Return history with string.
std_str Physics::extract_log_rect(
	std_str cDelim = "\t", std_str linbreak = "\n")
{
	std_str trace_record("");
	trace_record = trace_record + \
		"\"Coord X\"" + cDelim + "\"Coord Y\"" \
		+ cDelim + "\"Velocity X\"" + cDelim + "\"Velocity Y\"" \
		+ cDelim + "\"Mass (g)\"" + cDelim + "\"Reflected\"" \
		+ cDelim + "\"Time\"" + linbreak;

	ulong log_size = time_trace.size();

	for (ulong i = 0; i < log_size; i++) {
		trace_record = trace_record + \
			Converters::numtostdstr(x_loc.at(i)) + cDelim + \
			Converters::numtostdstr(y_loc.at(i)) + cDelim + \
			Converters::numtostdstr(x_vel.at(i)) + cDelim + \
			Converters::numtostdstr(y_vel.at(i)) + cDelim + \
			Converters::numtostdstr(curr_object->read_mass()) + cDelim + \
			Converters::btostdstryesno(reflected_status.at(i)) + cDelim + \
			Converters::numtostdstr(time_trace.at(i)) + \
			linbreak;
	}

	return trace_record;
}


// Calculate reflecting molecule in a limited space.
void Physics::reflect_rect(double time_frame)
{
	if (x_loc.back() > edge_right || \
		x_loc.back() < edge_left || \
		y_loc.back() > edge_top || \
		y_loc.back() < edge_bottom) {	
		throw invalid_argument(
			"The trace already found at the outside of \
			the boundary. Cannot proceed!!\n\n");
		exit(1);
	}

	double delta_t = time_frame*1e-4;
	double div_time = 0.0;
	std_vec_d adv_loc(2);
	
	do {
		adv_loc = proj_loc_rect(delta_t);		
		
		if (adv_loc[0] <= edge_left) {
			time_elapsed += delta_t;
			update_status(edge_left, adv_loc[1], \
				(-1)*curr_object->xv(), curr_object->yv(), \
				true);
		}
		if (adv_loc[0] >= edge_right) {
			time_elapsed += delta_t;
			update_status(edge_right, adv_loc[1], \
				(-1)*curr_object->xv(), curr_object->yv(), \
				true);
		}
		if (adv_loc[1] <= edge_bottom) {
			time_elapsed += delta_t;
			update_status(adv_loc[0], edge_bottom, \
				curr_object->xv(), (-1)*curr_object->yv(), \
				true);
		}
		if (adv_loc[1] >= edge_top) {
			time_elapsed += delta_t;
			update_status(adv_loc[0], edge_top, \
				curr_object->xv(), (-1)*curr_object->yv(), \
				true);
		}
		if (adv_loc[0] > edge_left && \
			adv_loc[0] < edge_right && \
			adv_loc[1] > edge_bottom && \
			adv_loc[1] < edge_top) {
			
			time_elapsed += delta_t;
			curr_object->set_location(adv_loc[0], adv_loc[1]);
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
	Seed_Rand();

	time_elapsed = 0.0;
	time_scale = 1.0; // Default 1 sec time interval.
	curr_object = Thing;
	curr_photon = NULL;
	rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	x_loc.push_back(obj_info[0]);
	y_loc.push_back(obj_info[1]);
	x_vel.push_back(obj_info[2]);
	y_vel.push_back(obj_info[3]);
	obj_mass = obj_info[4];

	time_trace.push_back(time_elapsed);

	reflected_status.push_back(false);

	b_verbose = false;

	Validate_Data();
}

Physics::Physics(Molecule* Thing, bool verbose)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	Seed_Rand();

	time_elapsed = 0.0;
	time_scale = 1.0; // Default 1 sec time interval.
	curr_object = Thing;
	curr_photon = NULL;
	rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	x_loc.push_back(obj_info[0]);
	y_loc.push_back(obj_info[1]);
	x_vel.push_back(obj_info[2]);
	y_vel.push_back(obj_info[3]);
	obj_mass = obj_info[4];

	time_trace.push_back(time_elapsed);

	reflected_status.push_back(false);

	b_verbose = verbose;

	Validate_Data();
}

Physics::Physics(
	Molecule* Thing, \
	double time_lim, double time_sc)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	Seed_Rand();

	time_elapsed = 0.;
	time_limit = time_lim;
	time_scale = time_sc;
	curr_object = Thing;
	curr_photon = NULL;
	rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	x_loc.push_back(obj_info[0]);
	y_loc.push_back(obj_info[1]);
	x_vel.push_back(obj_info[2]);
	y_vel.push_back(obj_info[3]);
	obj_mass = obj_info[4];

	time_trace.push_back(time_elapsed);

	reflected_status.push_back(false);

	b_verbose = false;

	Validate_Data();
}

Physics::Physics(
	Molecule* Thing, \
	double time_lim, double time_sc, \
	bool verbose)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	Seed_Rand();

	time_elapsed = 0.;
	time_limit = time_lim;
	time_scale = time_sc;
	curr_object = Thing;
	curr_photon = NULL;
	rand_type = 1;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	x_loc.push_back(obj_info[0]);
	y_loc.push_back(obj_info[1]);
	x_vel.push_back(obj_info[2]);
	y_vel.push_back(obj_info[3]);
	obj_mass = obj_info[4];

	time_trace.push_back(time_elapsed);

	reflected_status.push_back(false);

	b_verbose = verbose;

	Validate_Data();
}

Physics::Physics(
	Molecule* Thing, \
	double time_lim, double time_sc, \
	bool verbose, \
	unint rnd_type = 1)
	: edge_left(-1000.), edge_right(1000.),
	edge_top(1000), edge_bottom(-1000)
{
	Seed_Rand();
	
	time_elapsed = 0.;
	time_limit = time_lim;
	time_scale = time_sc;
	curr_object = Thing;
	curr_photon = NULL;
	rand_type = rnd_type;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	x_loc.push_back(obj_info[0]);
	y_loc.push_back(obj_info[1]);
	x_vel.push_back(obj_info[2]);
	y_vel.push_back(obj_info[3]);
	obj_mass = obj_info[4];

	time_trace.push_back(time_elapsed);

	reflected_status.push_back(false);

	b_verbose = verbose;

	Validate_Data();
}

Physics::Physics(
	Molecule* Thing, \
	double time_lim, double time_sc, \
	double rect_left, double rect_right, \
	double rect_top, double rect_bottom, \
	bool verbose, \
	unint rnd_type = 1)
{
	Seed_Rand();
	
	if (set_dimension_rect(
		rect_left, rect_right, rect_top, rect_bottom) != true) {
		throw invalid_argument(
			"Un oh... wrong dimension detected!!");
	}

	time_elapsed = 0.;
	time_limit = time_lim;
	time_scale = time_sc;
	curr_object = Thing;
	curr_photon = NULL;
	rand_type = rnd_type;

	vector<double> obj_info(5);
	obj_info = Thing->read_info();
	x_loc.push_back(obj_info[0]);
	y_loc.push_back(obj_info[1]);
	x_vel.push_back(obj_info[2]);
	y_vel.push_back(obj_info[3]);
	obj_mass = obj_info[4];

	time_trace.push_back(time_elapsed);

	reflected_status.push_back(false);

	b_verbose = verbose;

	Validate_Data();
}

Physics::~Physics()
{
	x_loc.clear();
	y_loc.clear();
	x_vel.clear();
	y_vel.clear();
	curr_object = NULL;
	curr_photon = NULL;

	time_trace.clear();
	reflected_status.clear();
}

#endif
