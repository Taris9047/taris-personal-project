/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T07:55:04-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T10:16:05-06:00
*/

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

#include "list.hpp"
#include "btree.hpp"
#include "utils.hpp"

/* Enumeration */
enum MNodeDir {RH, RD, DN, LH, LU, UP};

/* Matrix for pathfinder (Lefty route) */
const enum MNodeDir MPathFL[6][6] = \
{
	{LU, UP, RH, RD, DN, LH}, /* this to RH */
	{UP, RH, RD, DN, LH, LU}, /* this to RD */
	{RH, RD, DN, LH, LU, UP}, /* this to DN */
	{RD, DN, LH, LU, UP, RH}, /* this to LH */
	{DN, LH, LU, UP, RH, RD}, /* this to LU */
	{LH, LU, UP, RH, RD, DN}  /* this to UP */
};

/* Matrix for pathfinder (Righty route) */
const enum MNodeDir MPathFR[6][6] = \
{
	{DN, RD, RH, UP, LU, LH},
	{RD, RH, UP, LU, LH, DN},
	{RH, UP, LU, LH, DN, RD},
	{UP, LU, LH, DN, RD, RH},
	{LU, LH, DN, RD, RH, UP},
	{LH, DN, RD, RH, UP, LU}
};


/* The node */
template <class T, class KeyT>
class MNode : public std::enable_shared_from_this<MNode<T, KeyT>>
{
private:
	/* Pointer to the data */
	std::shared_ptr<T> data;
	/* Key */
	KeyT key;

	/* Detach all */
	int detach_all(enum MNodeDir dir);

	/* Attach */
	int attach(
		enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn);
	int update_L(
		enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn);
	int update_R(
		enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn);
	int update_adjs(
		enum MNodeDir attach_direction, std::shared_ptr<MNode<T, KeyT>> tn);

	/* Pathfinders */
	int find_L(
		enum MNodeDir in_dir, std::shared_ptr<MNode<T, KeyT>> n);
	int find_R(
		enum MNodeDir in_dir, std::shared_ptr<MNode<T, KeyT>> n);
	int find_me(std::shared_ptr<MNode<T, KeyT>> m);

	/* Map enum to node */
	std::shared_ptr<MNode<T, KeyT>> map(enum MNodeDir n, std::shared_ptr<MNode<T, KeyT>> nt);

public:
	/* Adjacent nodes */
	std::shared_ptr<MNode<T, KeyT>> rh;
	std::shared_ptr<MNode<T, KeyT>> rd;
	std::shared_ptr<MNode<T, KeyT>> dn;
	std::shared_ptr<MNode<T, KeyT>> lh;
	std::shared_ptr<MNode<T, KeyT>> lu;
	std::shared_ptr<MNode<T, KeyT>> up;

	/* Access... */
	int Set(T& d);
	int Set(std::shared_ptr<T> pd);
	T& Get();
	std::shared_ptr<T> pGet();

	void SetKey(KeyT& k);
	KeyT& GetKey();

	/* Manipulation */
	int SetRH(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetRD(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetDN(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetLH(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetLU(std::shared_ptr<MNode<T, KeyT>> mn);
	int SetUP(std::shared_ptr<MNode<T, KeyT>> mn);

	/* Operator overloading */
	MNode<T, KeyT>& operator= (const MNode<T, KeyT>& mn);
	MNode<T, KeyT>& operator= (MNode<T, KeyT>&& mn) noexcept;

	/* Constructors and Destructors */
	MNode();
	MNode(T& d, KeyT& k);
	MNode(std::shared_ptr<T> pd, KeyT& k);
	MNode(const MNode<T, KeyT>& mn);
	MNode(MNode<T, KeyT>&& mn) noexcept;
	virtual ~MNode();
};
















/* Node key, (default implementation) */
template <typename T>
struct NodeKey {
	T x;
	T y;

	/* Operator overloadings */
	bool operator== (const NodeKey<T>& o) const
	{
		if (x == o.x && y == o.y) return true;
		else return false;
	}

	bool operator!= (const NodeKey<T>& o) const
	{
		return !(this == o);
	}

	bool operator< (const NodeKey<T>& o) const
	{
		if ( x < o.x ) return true;
		else if ( x == o.x ) {
			if ( y < o.y ) return true;
			else return false;
		}
		else return false;
	}

	bool operator<= (const NodeKey<T>& o) const
	{
		return (this < o || this == o);
	}

	bool operator> (const NodeKey<T>& o) const
	{
		if ( x > o.x ) return true;
		else if ( x == o.x ) {
			if ( y > o.y ) return true;
			else return false;
		}
		else return false;
	}

	bool operator>= (const NodeKey<T>& o) const
	{
		return (this > o || this == o);
	}
};








#include "mesh.tcc"

#endif /* Include guard */
