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
void Photon::set_velocity(double xv, double yv)
{
	double vel_vec = sqrt(pow(xv,2)+pow(yv,2));
	this->current_velocity.xv = \
		LIGHT_SPEED*xv/vel_vec;
	this->current_velocity.yv = \
		LIGHT_SPEED*yv/vel_vec;
}

void Photon::set_location(double x, double y)
{
	this->current_coord.x = x;
	this->current_coord.y = y;
}

// Setting up physicsl properties
// equations that are useful
// E = hf
// lambda = c/f
void Photon::set_wavelength(double lambda)
{
	this->m_lambda = lambda;
	this->m_freq = this->m_lambda/this->current_velocity.vel;
	this->m_energy = PLANK_RECUDED_EV*this->m_freq;
}

void Photon::set_energy(double e)
{
	this->m_energy = e;
	this->m_freq = this->m_energy/PLANK_RECUDED_EV;
	this->m_lambda = this->current_velocity.vel/this->m_freq;
}

void Photon::set_frequency(double freq)
{
	this->m_freq = freq;
	this->m_lambda = this->current_velocity.vel/m_freq;
	this->m_energy = PLANK_RECUDED_EV*this->m_freq;
}

void Photon::set_light_speed(double light_spd)
{
	double curr_spd = sqrt(\
		pow(this->current_velocity.xv,2) + \
		pow(this->current_velocity.yv,2));
	this->current_velocity.vel = light_spd;
	this->current_velocity.xv = this->current_velocity.xv/curr_spd*light_spd;
	this->current_velocity.yv = this->current_velocity.yv/curr_spd*light_spd; 
}

void Photon::print_info()
{
	cout << endl;
	cout <<"Location: (" << this->current_coord.x << ", " << this->current_coord.y << ")" << endl;
	cout << "Velocity: <" << this->current_velocity.xv << ", " << this->current_velocity.yv << ">" << endl;
	cout << "Frequency: " << this->m_freq << " Hz" << endl;
	cout << endl;
}

std_vec_d Photon::read_info()
{
	std_vec_d info(7);

	info[0] = this->current_coord.x;
	info[1] = this->current_coord.y;
	info[2] = this->current_velocity.xv;
	info[3] = this->current_velocity.yv;
	info[4] = this->m_freq;
	info[5] = this->m_energy;
	info[6] = this->m_lambda;

	return info;
}

// Reading parameters one by one.
double Photon::x()
{
	return this->current_coord.x;
}

double Photon::y()
{
	return this->current_coord.y;
}

double Photon::xv()
{
	return this->current_velocity.xv;
}

double Photon::yv()
{
	return this->current_velocity.yv;
}

double Photon::vel()
{
	return this->current_velocity.vel;
}

double Photon::energy()
{
	return this->m_energy;
}

double Photon::freq()
{
	return this->m_freq;
}

double Photon::wavelength()
{
	return this->m_lambda;
}

// Constructors and Destructors
Photon::Photon()
{
	this->current_coord.x = 0;
	this->current_coord.y = 0;
	this->current_velocity.vel = LIGHT_SPEED;
	this->set_velocity(1., 1.);
	this->set_frequency(5000000);
}

Photon::Photon(double x, double y, double xv, double yv, double freq)
{
	this->current_coord.x = x;
	this->current_coord.y = y;
	this->current_velocity.vel = LIGHT_SPEED;
	this->set_velocity(xv, yv);
	this->set_frequency(freq);
}

Photon::~Photon()
{
}

#endif