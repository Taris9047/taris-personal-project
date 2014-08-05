#ifndef MOLECULE_HPP
#define MOLECULE_HPP

typedef std::vector<float> std_vec_f;

class Molecule
{
public:
  	float mass;
	
	struct coord2D
	{
		float x;
		float y;
	} current_coord;

	struct velocity2D
	{
		float xv;
		float yv;
		float vel;
	} current_velocity;

	// Constructors and Destructors
  	Molecule();
  	Molecule(float m);
  	Molecule(float x, float y, float xv, float yv, float m);
  	~Molecule();

  	void set_location(float x, float y);
	void set_velocity(float xv, float yv);
	void set_mass(float m);

	// Prints out current location, velocity, unity vector
	void print_info();
	std_vec_f read_info();
};

#endif
