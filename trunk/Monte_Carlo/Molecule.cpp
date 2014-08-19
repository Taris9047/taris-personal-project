#ifndef MOLECULE_CPP
#define MOLECULE_CPP

#include <iostream>
#include <cmath>
#include <vector>
#include "Molecule.hpp"

using namespace std;

// Manipulation methods
void Molecule::set_velocity(double xv, double yv)
{
	this->current_velocity.xv = xv;
	this->current_velocity.yv = yv;
	this->current_velocity.vel = sqrt(xv*xv + yv*yv);
}

void Molecule::set_location(double x, double y)
{
	this->current_coord.x = x;
	this->current_coord.y = y;
}

void Molecule::set_mass(double m)
{
	this->m_mass = m;
}

void Molecule::set_x(double x)
{
	this->current_coord.x = x;
}

void Molecule::set_y(double y)
{
	this->current_coord.y = y;
}

void Molecule::set_xv(double xv)
{
	this->current_velocity.xv = xv;
	this->current_velocity.vel = \
		sqrt(this->current_velocity.xv*this->current_velocity.xv + \
			this->current_velocity.yv*this->current_velocity.yv);
}

void Molecule::set_yv(double yv)
{
	this->current_velocity.yv = yv;
	this->current_velocity.vel = \
		sqrt(this->current_velocity.xv*this->current_velocity.xv + \
		this->current_velocity.yv*this->current_velocity.yv);
}

// Reporting status into console
void Molecule::print_info()
{
	cout << endl;
	cout <<"Location: (" << this->current_coord.x << ", " << this->current_coord.y << ")" << endl;
	cout << "Velocity: <" << this->current_velocity.xv << ", " << this->current_velocity.yv << ">" << endl;
	cout << "Mass: " << this->m_mass << " g" << endl;
	cout << endl;
}

std_vec_d Molecule::read_info()
{
	std_vec_d info(5);

	info[0] = this->current_coord.x;
	info[1] = this->current_coord.y;
	info[2] = this->current_velocity.xv;
	info[3] = this->current_velocity.yv;
	info[4] = this->m_mass;

	return info;
}

// Reading parameters one by one.
double Molecule::x()
{
	return this->current_coord.x;
}

double Molecule::y()
{
	return this->current_coord.y;
}

double Molecule::xv()
{
	return this->current_velocity.xv;
}

double Molecule::yv()
{
	return this->current_velocity.yv;
}

double Molecule::vel()
{
	return this->current_velocity.vel;
}

double Molecule::read_mass()
{
	return this->m_mass;
}



// Constructors and Destructors

Molecule::Molecule()
{
	this->current_coord.x = 0;
	this->current_coord.y = 0;
	this->current_velocity.xv = 0;
	this->current_velocity.yv = 0;
	this->current_velocity.vel = 0;
	this->m_mass = 0;
}

Molecule::Molecule(double m)
{
	this->current_coord.x = 0;
	this->current_coord.y = 0;
	this->current_velocity.xv = 0;
	this->current_velocity.yv = 0;
	this->current_velocity.vel = 0;
	this->m_mass = m;
}

Molecule::Molecule(double x, double y, double xv, double yv, double m)
{
	this->current_coord.x = x;
	this->current_coord.y = y;
	this->current_velocity.xv = xv;
	this->current_velocity.yv = yv;
	this->current_velocity.vel = sqrt(xv*xv + yv*yv);
	this->m_mass = m;
}

Molecule::~Molecule()
{
}

#endif