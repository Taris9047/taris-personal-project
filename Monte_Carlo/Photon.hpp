#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "data_types.hpp"

class Photon
{
private:
  	double m_energy;
  	double m_lambda;
  	double m_freq;

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
	void set_wavelength(double m);
	// Set energy as electron volt.
	void set_energy(double e);
	void set_frequency(double freq);
	void set_light_speed(double light_spd);

	// read up the information.
	double x();
	double y();
	double xv();
	double yv();
	double vel();
	double energy();
	double freq();
	double wavelength();

	// Prints out current location, velocity, unity vector
	void print_info();
	std_vec_d read_info();

	// Constructors and Destructors
  	Photon();
  	Photon(double energy);
  	Photon(double x, double y, double xv, double yv, double m);
  	~Photon();
};

#endif
