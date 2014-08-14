#ifndef RANDOM_HPP
#define RANDOM_HPP

class Random
{
public:
	void Seed_Rand();

	double uniform(double min, double max);
	double gaussian(double mean, double sigma, double amplitude);

	Random();
};

#endif