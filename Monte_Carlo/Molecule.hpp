#ifndef MOLECULE_HPP
#define MOLECULE_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include "data_types.hpp"

class Molecule
{
private:
  	double m_mass;
	
	struct coord2D
	{
		double x;
		double y;
	} current_coord;

	struct velocity2D
	{
		double xv;
		double yv;
		double vel;
	} current_velocity;

public:
  	// Manipulating status.
  	void set_location(double x, double y);
	void set_velocity(double xv, double yv);
	void set_mass(double m);
	void set_x(double x);
	void set_y(double y);
	void set_xv(double xv);
	void set_yv(double yv);

	// read up the information.
	double x();
	double y();
	double xv();
	double yv();
	double vel();
	double read_mass();

	// Prints out current location, velocity, unity vector
	void print_info();
	std_vec_d read_info();

	// Constructors and Destructors
  	Molecule();
  	Molecule(double m);
  	Molecule(double x, double y, double xv, double yv, double m);
  	~Molecule();
};

#endif
