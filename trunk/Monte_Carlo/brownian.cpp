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
	float unit_time = 1.;

	Molecule Hydrogen(1.00794*2.0*UNIT_MASS);
	Physics Hydrogen_rect(cal_time, unit_time, &Hydrogen);


	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}