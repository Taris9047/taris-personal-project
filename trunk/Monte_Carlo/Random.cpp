#ifndef RANDOM_CPP
#define RANDOM_CPP

#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Random.hpp"

// Probablity Density Functions
double Random::uniform(double min, double max)
{
	return ((max-min)*((double)rand()/RAND_MAX))+min;
}

double Random::gaussian(double mean, double sigma, double amplitude)
{
	return ((double)pow(-1,rand()%2))*amplitude*exp(-0.5*pow((this->uniform(mean-5*abs(sigma),mean+5*abs(sigma))-mean)/sigma,2));
}




void Random::Seed_Rand()
{
	srand((unsigned int) time(0));
}




// Constructors and Destructors
Random::Random()
{
	srand((unsigned int) time(0));
}

#endif