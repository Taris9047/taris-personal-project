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
#define CAL_TIME 100

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

	do {
		cout << "Time: " << cal_time*unit_time << " seconds:";

		// Reading out prev info.
		current_info = Hydrogen.read_info();

		// Updating parameters
		x = current_info[0]; y = current_info[1];
		float curr_mass = current_info[4];
		xv = (float) (rand()%BOUNDARY) * pow(-1, rand()%2);
		yv = (float) (rand()%BOUNDARY) * pow(-1, rand()%2);
		Hydrogen.set_velocity(xv, yv);

		current_info = Hydrogen.read_info();

		cout << endl;
		cout << x << ", " << y << endl;
		cout << xv << ", "	<< yv << endl;
		cout << curr_mass << " g" << endl;
		cout << endl;

		trace_record << x << "\t" << y \
			<< "\t" << xv << "\t" << yv \
			<< "\t" << curr_mass << "\t" << "NO" << endl;

		// Updating Coords. for next round.
		float x_inc; float y_inc; float x_n; float y_n;
		x_inc = xv*unit_time; y_inc = yv*unit_time;
		x_n = x+x_inc; y_n = y+y_inc;

		// Handling Reflection to the walls
		do {
			if (abs(x_n) >= BOUNDARY) {
				float diffx = abs(abs(x_n) - BOUNDARY);
				float remainx = abs(x_inc) - abs(diffx);
				float ratio_impact = abs(remainx/xv);
				float xv_r; float yv_r;
				
				// Updating the impact site.
				if (x_n >= BOUNDARY) {
					x_n = BOUNDARY;
				}
				else if (x_n <= BOUNDARY*(-1)) {
					x_n = BOUNDARY*(-1);
				}
				y_n = y+yv*ratio_impact;

				xv_r = xv*(1-ratio_impact)*(-1);
				yv_r = yv*(1-ratio_impact);

				cout << endl;
				cout << "** Reflection!! **" << endl;
				cout << x_n << ", " << y_n << endl \
					<< xv_r << ", "	<< yv_r << endl;
				cout << curr_mass << endl;
				cout << endl;

				// Update place of impact to the file.
				trace_record << x_n << "\t" << y_n << "\t" \
					<< xv_r << "\t" << yv_r << "\t" \
					<< curr_mass << "\t" << "YES" << endl;

				// Update Final Location
				x_n += xv_r; y_n += yv_r;

				Hydrogen.set_location(x_n, y_n);
			}

			else if (abs(y_n) >= BOUNDARY) {
				float diffy = abs(abs(y_n) - BOUNDARY);
				float remainy = abs(y_inc) - abs(diffy);
				float ratio_impact = abs(remainy/yv);
				float xv_r; float yv_r;
				
				// Updating the impact site.
				if (y_n >= BOUNDARY) {
					y_n = BOUNDARY;
				}
				else if (y_n <= BOUNDARY*(-1)) {
					y_n = BOUNDARY*(-1);
				}
				x_n = x+xv*ratio_impact;

				yv_r = yv*(1-ratio_impact)*(-1);
				xv_r = xv*(1-ratio_impact);

				cout << endl;
				cout << "** Reflection!! **" << endl;
				cout << x_n << ", " << y_n << endl \
					<< xv_r << ", "	<< yv_r << endl;
				cout << curr_mass << endl;
				cout << endl;

				// Update place of impact to the file.
				trace_record << x_n << "\t" << y_n << "\t" \
					<< xv_r << "\t" << yv_r << "\t" \
					<< curr_mass << "\t" << "YES" << endl;

				// Update Final Location
				x_n += xv_r; y_n += yv_r;

				Hydrogen.set_location(x_n, y_n);
			}
		} while (x_n >= BOUNDARY || y_n >= BOUNDARY);

		Hydrogen.set_location(x_n, y_n);

		cal_time++;

	} while (cal_time <= CAL_TIME);

	trace_record.close();
	cout << "File saved..." << endl;

	cout << "Press any key to continue..." << endl;
	cin.get();

	return 0;
}