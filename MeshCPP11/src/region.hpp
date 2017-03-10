/**
* @Author: Taylor Shin <SHINLT+kshin>
* @Date:   2017-03-09T13:32:59-06:00
* @Email:  kshin@aps.anl.gov
* @Last modified by:   SHINLT+kshin
* @Last modified time: 2017-03-09T13:33:32-06:00
*/



/****************************************************

 Data structure c++11

 Region header file

 Written by Taylor Shin

 Mar. 9th 2017

*****************************************************/

#ifndef CPP11_REGION_H
#define CPP11_REGION_H

#include <memory>

#include "utils.hpp"
#include "mesh.hpp"
#include "matrix.hpp"

template <class T>
class Region : public std::enable_shared_from_this<Region<T>>
{
private:
	std::unique_ptr<Matrix< std::shared_ptr<MNode<T, IndKey>> >> MNodes;
	std::shared_ptr<MNode<T, IndKey>> root_node;

	ULLONG rows;
	ULLONG cols;

	int make_nodes();

public:
	/* Access */
	std::unique_ptr<Matrix<T>> GetNodes() { return MNodes; }
	std::shared_ptr<MNode<T, IndKey>> GetRootNode() { return root_node; }
	ULLONG Rows() { return rows; }
	ULLONG Cols() { return cols; }

	/* Manipulation */
	void AssignData(T** data);
	void AssignData(std::vector<std::vector<T>> data);

	/* operator overloading */
	Region<T>& operator= (const Region<T>& r);
	Region<T>& operator= (Region<T>&& r) noexcept;

	/* Constructors and Destructors */
	Region();
	Region(const ULLONG& size_r, const ULLONG& size_c);
	Region(const Region<T>& r);
	Region(Region<T>&& r) noexcept;
	virtual ~Region();
};





#include "region.tcc"

#endif /* Include guard */
