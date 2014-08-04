#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
#include "Molecule.hpp"
#include "Physics.hpp"

#define UNIT_MASS 1.660538921E-25 
#define BOUNDARY 1000
#define VELOCITY_LIMIT 500
#define CAL_TIME 3600

using namespace std;

int main (void)
{
	srand(time(0));
	int cal_time = 0;
	float x, y, xv, yv;
	float unit_time = 1.;

	Molecule Hydrogen(1.00794*2.0*UNIT_MASS);
	float_vec_t current_info;
	ofstream trace_record;
	trace_record.open("trace.txt");
	trace_record << "\"Coord X\"" << "\t" << "\"Coord Y\"" \
		<< "\t" << "\"Velocity X\"" << "\t" << "\"Velocity Y\"" \
		<< "\t" << "\"Mass (g)\"" << "\t" << "\"Reflected\"" \
		<< endl;
	trace_record << scientific;

	Physics Hydrogen_rect(cal_time, unit_time, &Hydrogen);



	trace_record.close();
	cout << "File saved..." << endl;

	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}