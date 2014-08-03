#include "Molecule.hpp"

#ifndef PHYSICS_HPP
#define PHYSICS_HPP

// Physics implementation

// Currently, contains only reflection handler.

class Physics: public Molecule
{
public:
	void reflect_rect(float edge_x, float edge_y, float time_scale, Molecule* Thing);
};

#endif