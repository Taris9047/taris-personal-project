/****************************************************

 Data structure c++11

 Mesh datastructure header file

 Written by Taylor Shin

 Mar. 6th 2017

*****************************************************/

#ifndef CPP11_MESH_H
#define CPP11_MESH_H

#include <memory>
#include <vector>
#include <cassert>

#include "utils.h"

/* key */
template <typename T>
struct Key {
    T x_coord;
    T y_coord;
};

/* keygen */
template <typename T>
T Keygen(Key<T>& coord_key)
{
	T a = coord_key.x_coord;
	T b = coord_key.y_coord;
	return (a+b)*(a+b+1)/2+a;
}

/* Node definition */
template <class T, class KeyT>
class MNode {
private:
	std::shared_ptr<T> data; /* The actual data */
    Key<KeyT> coords; /* key */
	/*
	 	Index of adjnodes...
		0: right side
		1: right down
		2: down side
		3: left side
		4: upper left
		5: upper side
	*/
    std::vector<std::shared_ptr<MNode<T, KeyT>>> adjnodes;

public:
	/* Access methods */
	Key<KeyT> GetCoords() const;
	std::shared_ptr<MNode<T, KeyT>> GetRH() const;
	std::shared_ptr<MNode<T, KeyT>> GetRD() const;
	std::shared_ptr<MNode<T, KeyT>> GetDN() const;
	std::shared_ptr<MNode<T, KeyT>> GetLH() const;
	std::shared_ptr<MNode<T, KeyT>> GetLU() const;
	std::shared_ptr<MNode<T, KeyT>> GetUP() const;
	void SetCoords(Key<KeyT>& k);
	void SetRH(std::shared_ptr<MNode<T, KeyT>> pmn);
	void SetRD(std::shared_ptr<MNode<T, KeyT>> pmn);
	void SetDN(std::shared_ptr<MNode<T, KeyT>> pmn);
	void SetLH(std::shared_ptr<MNode<T, KeyT>> pmn);
	void SetLU(std::shared_ptr<MNode<T, KeyT>> pmn);
	void SetUP(std::shared_ptr<MNode<T, KeyT>> pmn);

	/* Constructors and Destructors */
    MNode();
	MNode(T& d, Key<KeyT>& k);
    MNode(MNode<T, KeyT>& mn); /* copy constructor */
    virtual ~MNode();
};





#include "mesh.tcc"

#endif /* Include guard */
