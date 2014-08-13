#ifndef MOLECULE_CPP
#define MOLECULE_CPP

#include <iostream>
#include <cmath>
#include <vector>
#include "Molecule.hpp"

using namespace std;

void Molecule::set_velocity(float xv, float yv)
{
	this->current_velocity.xv = xv;
	this->current_velocity.yv = yv;
	this->current_velocity.vel = sqrt(xv*xv + yv*yv);
}

void Molecule::set_location(float x, float y)
{
	this->current_coord.x = x;
	this->current_coord.y = y;
}

void Molecule::set_mass(float m)
{
	this->mass = m;
}

void Molecule::print_info()
{
	cout << endl;
	cout <<"Location: (" << this->current_coord.x << ", " << this->current_coord.y << ")" << endl;
	cout << "Velocity: <" << this->current_velocity.xv << ", " << this->current_velocity.yv << ">" << endl;
	cout << "Mass: " << this->mass << " g" << endl;
	cout << endl;
}

std_vec_f Molecule::read_info()
{
	std_vec_f info(5);

	info[0] = this->current_coord.x;
	info[1] = this->current_coord.y;
	info[2] = this->current_velocity.xv;
	info[3] = this->current_velocity.yv;
	info[4] = this->mass;

	return info;
}

// Reading parameters one by one.
float Molecule::x()
{
	return this->current_coord.x;
}

float Molecule::y()
{
	return this->current_coord.y;
}

float Molecule::xv()
{
	return this->current_velocity.xv;
}

float Molecule::yv()
{
	return this->current_velocity.yv;
}

float Molecule::vel()
{
	return this->current_velocity.vel;
}



// Constructors and Destructors

Molecule::Molecule()
{
	this->current_coord.x = 0;
	this->current_coord.y = 0;
	this->current_velocity.xv = 0;
	this->current_velocity.yv = 0;
	this->current_velocity.vel = 0;
	this->mass = 0;
}

Molecule::Molecule(float m)
{
	this->current_coord.x = 0;
	this->current_coord.y = 0;
	this->current_velocity.xv = 0;
	this->current_velocity.yv = 0;
	this->current_velocity.vel = 0;
	this->mass = m;
}

Molecule::Molecule(float x, float y, float xv, float yv, float m)
{
	this->current_coord.x = x;
	this->current_coord.y = y;
	this->current_velocity.xv = xv;
	this->current_velocity.yv = yv;
	this->current_velocity.vel = sqrt(xv*xv + yv*yv);
	this->mass = m;
}

Molecule::~Molecule()
{
}

#endif