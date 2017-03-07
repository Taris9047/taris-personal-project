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
typedef struct _key {
    T x_coord;
    T y_coord;
} Key;

/* Node definition */
template <class T, class KeyT>
class MNode {
private:
    Key<KeyT> coords;
    std::vector<std::shared_ptr<MNode<T, KeyT>> adjnodes;

public:
    MNode();
    MNode(MNode<T, KeyT>& mn);
    virtual ~MNode();
};





#include "mesh.tcc"

#endif /* Include guard */
