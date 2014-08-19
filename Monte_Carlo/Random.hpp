#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "data_types.hpp"

//typedef unsigned int unint;

class Random
{
private:
	double beta_f(double x, double y);
	double gamma_f(double t);
	unint factorial(unint n); // Do not use input over 100!!! it will crash!

public:
	// For... you know... redandancy. A seed generator, if needed.
	void Seed_Rand();

	// Uniform distribution
	double uniform(double min, double max);
	// Gaussian distribution (Normal distribution)
	double gaussian(double mean, double sigma, double amplitude);
	// Beta distribution (amplitude is always 1)
	double beta(double a, double b);
	// Binomial distribution
	double binomial(unint n, unint p);
	// Chi-square distribution
	double chi_square(double k);

	Random();
};

#endif