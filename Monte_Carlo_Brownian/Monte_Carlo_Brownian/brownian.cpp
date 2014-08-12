#include <iostream>
#include <cstdlib>
#include <string>
#include "Molecule.hpp"
#include "Physics.hpp"

#define UNIT_MASS 1.660538921E-25 
#define BOUNDARY 1000.
#define VELOCITY_LIMIT 500.
#define CAL_TIME 100.
#define UNIT_TIME 1.

using namespace std;

int main (void)
{
	int cal_time = CAL_TIME;
	float unit_time = UNIT_TIME;
	string log_filename = "trace.txt";

	Molecule Hydrogen(1.00794*2.0*UNIT_MASS);
	Physics Hydrogen_rect(cal_time, unit_time, &Hydrogen);
	Hydrogen_rect.brownian_rect(VELOCITY_LIMIT, VELOCITY_LIMIT, \
		-BOUNDARY, BOUNDARY, \
		BOUNDARY, -BOUNDARY);
	Hydrogen_rect.write_log_rect(log_filename);

	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}