#ifndef MOLECULE_HPP
#define MOLECULE_HPP

#include "data_types.hpp"

class Molecule
{
public:
  	double mass;
	
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

  	// Manipulating status.
  	void set_location(double x, double y);
	void set_velocity(double xv, double yv);
	void set_mass(double m);

	// read up the information.
	double x();
	double y();
	double xv();
	double yv();
	double vel();

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
