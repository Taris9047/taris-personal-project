#ifndef PHOTON_CPP
#define PHOTON_CPP

#include <iostream>
#include <cmath>
#include <vector>
#include "Photon.hpp"

// Reduced Plank's constant in Js
#define PLANK_REDUCED 1.054571726e-34 
// Reduced Plank's constant in eVs
#define PLANK_RECUDED_EV 6.58211928e-16
// Light speed m/s
#define LIGHT_SPEED 299792458.

using namespace std;

// According to reletivity theory, the photon speed in vacuum
// cannot change. Thus, velocity set function automatically
// converts the input vector to unity
//
// So, basically, input vector of this function does not imply
// the velocity of the photon itself but a ratio. 
//
void Photon::set_velocity(double xv, double yv)
{
	double vel_vec = sqrt(pow(xv,2)+pow(yv,2));
	current_velocity.xv = \
		LIGHT_SPEED*xv/vel_vec;
	current_velocity.yv = \
		LIGHT_SPEED*yv/vel_vec;
}

void Photon::set_location(double x, double y)
{
	current_coord.x = x;
	current_coord.y = y;
}

// Setting up physicsl properties
// equations that are useful
// E = hf
// lambda = c/f
void Photon::set_wavelength(double lambda)
{
	m_lambda = lambda;
	m_freq = m_lambda/current_velocity.vel;
	m_energy = PLANK_RECUDED_EV*m_freq;
}

void Photon::set_energy(double e)
{
	m_energy = e;
	m_freq = m_energy/PLANK_RECUDED_EV;
	m_lambda = current_velocity.vel/m_freq;
}

void Photon::set_frequency(double freq)
{
	m_freq = freq;
	m_lambda = current_velocity.vel/m_freq;
	m_energy = PLANK_RECUDED_EV*m_freq;
}

void Photon::set_light_speed(double light_spd)
{
	double curr_spd = sqrt(\
		pow(current_velocity.xv,2) + \
		pow(current_velocity.yv,2));
	current_velocity.vel = light_spd;
	current_velocity.xv = current_velocity.xv/curr_spd*light_spd;
	current_velocity.yv = current_velocity.yv/curr_spd*light_spd; 
}

void Photon::print_info()
{
	cout << endl;
	cout <<"Location: (" << current_coord.x << ", " << current_coord.y << ")" << endl;
	cout << "Velocity: <" << current_velocity.xv << ", " << current_velocity.yv << ">" << endl;
	cout << "Frequency: " << m_freq << " Hz" << endl;
	cout << endl;
}

std_vec_d Photon::read_info()
{
	std_vec_d info(7);

	info[0] = current_coord.x;
	info[1] = current_coord.y;
	info[2] = current_velocity.xv;
	info[3] = current_velocity.yv;
	info[4] = m_freq;
	info[5] = m_energy;
	info[6] = m_lambda;

	return info;
}

// Reading parameters one by one.
double Photon::x()
{
	return current_coord.x;
}

double Photon::y()
{
	return current_coord.y;
}

double Photon::xv()
{
	return current_velocity.xv;
}

double Photon::yv()
{
	return current_velocity.yv;
}

double Photon::vel()
{
	return current_velocity.vel;
}

double Photon::energy()
{
	return m_energy;
}

double Photon::freq()
{
	return m_freq;
}

double Photon::wavelength()
{
	return m_lambda;
}

// Constructors and Destructors
Photon::Photon()
{
	current_coord.x = 0;
	current_coord.y = 0;
	current_velocity.vel = LIGHT_SPEED;
	set_velocity(1., 1.);
	set_frequency(5000000);
}

Photon::Photon(double x, double y, double xv, double yv, double freq)
{
	current_coord.x = x;
	current_coord.y = y;
	current_velocity.vel = LIGHT_SPEED;
	set_velocity(xv, yv);
	set_frequency(freq);
}

Photon::~Photon()
{
}

#endif