/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Quad tree

  header file

**********************************************/
#ifndef CPP11_IMPLEMENTATION_DATASTR_QUAD_TREE_HEADER
#define CPP11_IMPLEMENTATION_DATASTR_QUAD_TREE_HEADER

#include <cstdint>
#include <memory>
#include <string>
#include <list>
#include <stdexcept>

#include "utils.hpp"

typedef enum { NorthEast, NorthWest, SouthEast, SouthWest } QTNodeSide;

template<class T>
class QTree {

protected:
	struct QTreeNode {
		struct QTreeNode* parent;

		std::unique_ptr<QTreeNode> NE;
		std::unique_ptr<QTreeNode> NW;
		std::unique_ptr<QTreeNode> SE;
		std::unique_ptr<QTreeNode> SW;

		T data;
		uint64_t index;

		void Assign(T& input_data) { data = input_data; }
		QTreeNode() :
			index(0), parent(nullptr),
			NE(nullptr), NW(nullptr), SE(nullptr), SW(nullptr)
		{;}
		virtual ~QTreeNode() {;}
	};

private:
	uint64_t depth;
	uint64_t n_nodes;
	std::unique_ptr<QTreeNode> root_node;

	std::unique_ptr<QTreeNode>& search(const uint64_t& index);


public:
	/* Methods */
	void Insert(const T& data, const uint64_t& index);
	T& Get(const uint64_t& index);

	/* Constructors and Destructors */
	QTree();
	virtual ~QTree();

}; /* class QTree */


#include "qtree.tcc"

#endif /* Include guard */
