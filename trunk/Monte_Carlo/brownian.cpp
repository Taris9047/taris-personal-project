#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Molecule.hpp"
#include "Physics.hpp"

#define UNIT_MASS 1.660538921E-25 
#define BOUNDARY_X 1000.
#define BOUNDARY_Y 1000.
#define VELOCITY_LIMIT_X 1000.
#define VELOCITY_LIMIT_Y 1000.
#define CAL_TIME 20.
#define UNIT_TIME 1.

using namespace std;

int main (int argc, char* argv[])
{
	int cal_time;
	double unit_time;

	if (argc == 2) {
		stringstream convert(argv[1]);
		if ( !(convert >> cal_time) ) {
			cal_time = CAL_TIME;
		}

		unit_time = UNIT_TIME;
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
	else {
		cal_time = CAL_TIME;
		unit_time = UNIT_TIME;
	}

	cout << "Calculation Time: " << cal_time*unit_time << endl;

	string log_filename = "trace.csv";
	double mass_hydrogen = 1.00794*2.0*UNIT_MASS;

	Molecule Hydrogen(mass_hydrogen);
	Physics Hydrogen_rect(cal_time, unit_time, &Hydrogen, true);
	Hydrogen_rect.brownian_rect(\
		VELOCITY_LIMIT_X, VELOCITY_LIMIT_Y, \
		-BOUNDARY_X, BOUNDARY_X, \
		BOUNDARY_Y, -BOUNDARY_Y);
	Hydrogen_rect.write_log_rect(log_filename, "\t");

	//cout << "Press Enter (Return) key to continue..." << endl;
	//cin.get();

	return 0;
}