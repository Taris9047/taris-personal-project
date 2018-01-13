#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>
#include "Molecule.hpp"
#include "Physics.hpp"
#include "Gnuplot_Gen.hpp"
#include "Converters.hpp"

#define UNIT_MASS 1.660538921E-25
#define BOUNDARY_LEFT -1000.
#define BOUNDARY_RIGHT 1000.
#define BOUNDARY_TOP 1000.
#define BOUNDARY_BOTTOM -1000.
#define VELOCITY_LIMIT_X (abs(BOUNDARY_LEFT)+abs(BOUNDARY_RIGHT))/3.
#define VELOCITY_LIMIT_Y (abs(BOUNDARY_TOP)+abs(BOUNDARY_BOTTOM))/3.
#define CAL_TIME 100.
#define UNIT_TIME 1.
#define RNG_TYPE 2.

using namespace std;

int main (int argc, char* argv[])
{
	double cal_time;
	double unit_time;
	unsigned int rng_type;

	// Initializing input parameters.
	cal_time = CAL_TIME;
	unit_time = UNIT_TIME;
	rng_type = RNG_TYPE;

	if (argc == 2) {
		stringstream convert(argv[1]);
		if ( !(convert >> cal_time) ) {
			cal_time = CAL_TIME;
		}
	}
	else if (argc == 3) {
		stringstream converta(argv[1]);
		if ( !(converta >> cal_time) ) {
			cal_time = CAL_TIME;
		}

		stringstream convertb(argv[2]);
		if ( !(convertb >> unit_time)) {
			unit_time = UNIT_TIME;
		}	
	}
	else if (argc == 4) {
		stringstream converta(argv[1]);
		if ( !(converta >> cal_time) ) {
			cal_time = CAL_TIME;
		}
		stringstream convertb(argv[2]);
		if ( !(convertb >> unit_time)) {
			unit_time = UNIT_TIME;
		}
		stringstream convertc(argv[3]);
		if ( !(convertc >> rng_type)) {
			rng_type = RNG_TYPE;
		}
	}

	cout << "Calculation Time: " << cal_time*unit_time << endl;

	std_str log_filename = "trace.csv";
	std_str fig_filename = "trace.eps";
	std_str gPlot_filename = "plot.gp";
	std_str fig_title = "Brownian movement for: " + \
		Converters::numtostdstr(cal_time) + " seconds.";

	double mass_hydrogen = 1.00794*2.0*UNIT_MASS;
	Molecule Hydrogen(mass_hydrogen);
	GnuplotGen Hydrogen_rect_plot(
		"plot.gp", log_filename, \
		fig_filename, fig_title, \
		BOUNDARY_LEFT, BOUNDARY_RIGHT, \
		BOUNDARY_TOP, BOUNDARY_BOTTOM);

	Hydrogen_rect_plot.set_dimension_rect(
		BOUNDARY_LEFT, BOUNDARY_RIGHT, \
		BOUNDARY_TOP, BOUNDARY_BOTTOM);
	Physics Hydrogen_rect(
		&Hydrogen, cal_time, unit_time, \
		true, rng_type);

	Hydrogen_rect.Reset_Sim();
	
	Hydrogen_rect.set_dimension_rect(
		BOUNDARY_LEFT, BOUNDARY_RIGHT, \
		BOUNDARY_TOP, BOUNDARY_BOTTOM);
	
	Hydrogen_rect.brownian_rect(
		VELOCITY_LIMIT_X, VELOCITY_LIMIT_Y);
	
	Hydrogen_rect.write_log_rect(
		log_filename, ",", "\n");

	cout << endl;
	cout << "Generating Gnuplot input deck." \
		<< endl;
	Hydrogen_rect_plot.WriteDeck();
	cout << endl;
	cout << "Press Enter (Return) key to continue..." \
		<< endl;

	cin.get();

	return 0;
}
