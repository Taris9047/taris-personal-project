#ifndef MOLECULE_CPP
#define MOLECULE_CPP

#include "Molecule.hpp"

using namespace std;

// Manipulation methods
void Molecule::set_velocity(double xv, double yv)
{
	current_velocity.xv = xv;
	current_velocity.yv = yv;
	current_velocity.vel = sqrt(xv*xv + yv*yv);
}

void Molecule::set_location(double x, double y)
{
	current_coord.x = x;
	current_coord.y = y;
}

void Molecule::set_mass(double m)
{
	m_mass = m;
}

void Molecule::set_x(double x)
{
	current_coord.x = x;
}

void Molecule::set_y(double y)
{
	current_coord.y = y;
}

void Molecule::set_xv(double xv)
{
	current_velocity.xv = xv;
	current_velocity.vel = \
		sqrt(current_velocity.xv*current_velocity.xv + \
			current_velocity.yv*current_velocity.yv);
}

void Molecule::set_yv(double yv)
{
	current_velocity.yv = yv;
	current_velocity.vel = \
		sqrt(current_velocity.xv*current_velocity.xv + \
		current_velocity.yv*current_velocity.yv);
}

// Reporting status into console
void Molecule::print_info()
{
	cout << endl;
	cout <<"Location: (" << current_coord.x << ", " << current_coord.y << ")" << endl;
	cout << "Velocity: <" << current_velocity.xv << ", " << current_velocity.yv << ">" << endl;
	cout << "Mass: " << m_mass << " g" << endl;
	cout << endl;
}

std_vec_d Molecule::read_info()
{
	std_vec_d info(5);

	info[0] = current_coord.x;
	info[1] = current_coord.y;
	info[2] = current_velocity.xv;
	info[3] = current_velocity.yv;
	info[4] = m_mass;

	return info;
}

// Reading parameters one by one.
double Molecule::x()
{
	return current_coord.x;
}

double Molecule::y()
{
	return current_coord.y;
}

double Molecule::xv()
{
	return current_velocity.xv;
}

double Molecule::yv()
{
	return current_velocity.yv;
}

double Molecule::vel()
{
	return current_velocity.vel;
}

double Molecule::read_mass()
{
	return m_mass;
}



// Constructors and Destructors

Molecule::Molecule()
{
	current_coord.x = 0;
	current_coord.y = 0;
	current_velocity.xv = 0;
	current_velocity.yv = 0;
	current_velocity.vel = 0;
	m_mass = 0;
}

Molecule::Molecule(double m)
{
	current_coord.x = 0;
	current_coord.y = 0;
	current_velocity.xv = 0;
	current_velocity.yv = 0;
	current_velocity.vel = 0;
	m_mass = m;
}

Molecule::Molecule(double x, double y, double xv, double yv, double m)
{
	current_coord.x = x;
	current_coord.y = y;
	current_velocity.xv = xv;
	current_velocity.yv = yv;
	current_velocity.vel = sqrt(xv*xv + yv*yv);
	m_mass = m;
}

Molecule::~Molecule()
{
}

#endif