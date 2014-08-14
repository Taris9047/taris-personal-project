#ifndef PHOTON_HPP
#define PHOTON_HPP

#include "data_types.hpp"

class Photon
{
private:
  	float m_energy;
  	float m_lambda;
  	float m_freq;

	struct coord2D
	{
		float x;
		float y;
	} current_coord;

	struct velocity2D
	{
		float xv;
		float yv;
		float vel;
	} current_velocity;

public:
  	// Manipulating status.
  	void set_location(float x, float y);
	void set_velocity(float xv, float yv);
	void set_wavelength(float m);
	// Set energy as electron volt.
	void set_energy(float e);
	void set_frequency(float freq);
	void set_light_speed(float light_spd);

	// read up the information.
	float x();
	float y();
	float xv();
	float yv();
	float vel();
	float energy();
	float freq();
	float wavelength();

	// Prints out current location, velocity, unity vector
	void print_info();
	std_vec_f read_info();

	// Constructors and Destructors
  	Photon();
  	Photon(float energy);
  	Photon(float x, float y, float xv, float yv, float m);
  	~Photon();
};

#endif
