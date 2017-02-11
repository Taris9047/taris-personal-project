#ifndef RANDOM_CPP
#define RANDOM_CPP

#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Random.hpp"

#include <iostream>

// Needs adjustment for better precision. Basically, this is Infinity!
#define INF 1E10 
#define FACTORIAL_MAX 20

//////////////////////////////////
// Probablity Density Functions //
//////////////////////////////////

// Uniform distribution (just an RNG)
double Random::uniform(double min, double max)
{
	return ((max-min)*((double)rand()/RAND_MAX))+min;
}

// Gaussian distribution
double Random::gaussian(double mean, double sigma, double amplitude = 1.)
{
	return ((double)pow(-1,rand()%2))*amplitude*exp(-0.5*pow((this->uniform(mean-5*abs(sigma),mean+5*abs(sigma))-mean)/sigma,2));
}

// Beta distribution
double Random::beta(double a, double b, double amplitude = 1.)
{
	double x = this->uniform(0, 1.0);
	return amplitude*p_bipolar()*pow(x, a-1)*pow(1-x, b-1)/this->beta_f(a, b);
}

// Binomial distribution
double Random::binomial(ullong n, ullong k, double amplitude = 1.)
{
	if (k > n)
		k = n;

	// Adjusting number to avoid overflow
	if (n > FACTORIAL_MAX)
		n = FACTORIAL_MAX;
	if (k > FACTORIAL_MAX)
		k = FACTORIAL_MAX;

	double p = this->p();

	return amplitude*((double)this->p_bipolar()*this->factorial(n)/(factorial(k)*factorial(n-k))*pow(p,n)*pow(1-p,n-k));
}

// Chi-square distribution
double Random::chi_square(double k, double amplitude)
{
	double x = this->uniform(0, INF);

	if (x < 0)
		return 0;
	else
		return amplitude*(1/(pow(2, k/2)*this->gamma_f(k/2)))*pow(x,(k/2)-1)*exp(x/(-2));
}

// Poisson distribution
double Random::poisson(double lambda, ullong k, double amplitude = 1.)
{
	if (k > FACTORIAL_MAX)
		k = FACTORIAL_MAX;

	return (double) amplitude*pow(lambda,(double)k)*exp(lambda*(-1))/(double)factorial(k);

}




//////////////////////////
//      Utilities       //
//////////////////////////

// Seed Generator
void Random::Seed_Rand()
{
	srand((unsigned int) time(0));
}

// Factorial
ullong Random::factorial(ullong n)
{
	if (n <= 1)
		return 1;
	else 
		return this->factorial(n-1)*n;
}


/////////////////////
// Private Methods //
/////////////////////

// Beta Function
double Random::beta_f(double x, double y)
{
	const double dt = 1e-4;
	double t = 0.0; // Starts integration from 0.0
	double beta_int = 0.0;

	do {
		beta_int += (pow(t,x-1)*pow(1-t,y-1))*dt;
		t += dt;
	} while (t <= 1.0);

	return beta_int;
}

// Gamme Function
double Random::gamma_f(double t)
{
	const double dx = 1e-4;
	double x = 0.0; // Starts integration from 0.0
	double gamma_int = 0.0;

	do {
		gamma_int += pow(x, t-1)*exp(-x)*dx;
		x += dx;
	} while (t <= 20); 

	return gamma_int;
}

// p: returns 0 or 1
ullong Random::p()
{
	return rand()%2;
}

// p_bipolar: returns -1, 0, or 1
long Random::p_bipolar()
{
	return (long)pow(-1, rand()%2)*rand()%2;
}


// Constructors and Destructors
Random::Random()
{
	this->Seed_Rand();
}

Random::~Random()
{
}

#endif