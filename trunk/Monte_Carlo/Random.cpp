#ifndef RANDOM_CPP
#define RANDOM_CPP

#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Random.hpp"

// Needs adjustment for better precision. Basically, this is Infinity!
#define INF 1E15 

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
	return amplitude*(1/this->beta_f(a, b))*pow(x, a-1)*pow(1-x, b-1);
}

// Binomial distribution
double Random::binomial(unint n, unint p)
{
	unint x = rand()%100;

	return this->factorial(n)/(factorial(x)*factorial(n-x))*pow(p,x)*pow(1-p,n-x);
}

// Chi-square distribution
double Random::chi_square(double k)
{
	double x = this->uniform(-20, 20);
	return (1/(pow(2, k/2)*this->gamma_f(k/2)))*pow(x,(k/2)-1)*exp(x/(-2));
}

///////////////
// Utilities //
///////////////

// Seed Generator
void Random::Seed_Rand()
{
	srand((unsigned int) time(0));
}

// Factorial
unint Random::factorial(unint n)
{
	if (n == 0) {
		return 1;
	}
	else {
		return this->factorial(n-1)*n;
	}
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


// Constructors and Destructors
Random::Random()
{
	this->Seed_Rand();
}

Random::~Random()
{
}

#endif