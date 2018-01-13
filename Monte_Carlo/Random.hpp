#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "data_types.hpp"

//typedef unsigned int unint;

class Random
{
private:
	double beta_f(double x, double y);
	double gamma_f(double t);
	ullong factorial(ullong n); // Do not use an input over 100!!! it will crash!
	ullong p(); // returns 0 or 1
	long p_bipolar(); // returns 0, 1, or -1

public:
	// For... you know... redandancy. A seed generator, if needed.
	void Seed_Rand();

	// Uniform distribution
	double uniform(double min, double max);
	// Gaussian distribution (Normal distribution)
	double gaussian(double mean, double sigma, double amplitude);
	// Beta distribution 	
	double beta(double a, double b, double amplitude);
	// Binomial distribution
	double binomial(ullong n, ullong p, double amplitude);
	// Chi-square distribution
	double chi_square(double k, double amplitude);
	// Poisson distribution
	double poisson(double lambda, ullong k, double amplitude);

	Random();
	~Random();
};

#endif