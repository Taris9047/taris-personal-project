/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Quad tree

  header file

**********************************************/
#ifndef CPP11_IMPLEMENTATION_DATASTR_QUAD_TREE_HEADER
#define CPP11_IMPLEMENTATION_DATASTR_QUAD_TREE_HEADER

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <list>
#include <stdexcept>
#include <set>

#include "utils.hpp"

typedef enum { NorthEast, NorthWest, SouthEast, SouthWest } QTNodeSide;

template<class T>
class QTree {

protected:

	struct QTreeNode {

		struct QTreeNode* parent;

		T data;
		uint64_t index;

		bool operator< (const QTreeNode& other) const
		{ return this->index < other.index; }
		bool operator> (const QTreeNode& other) const
		{ return this->index > other.index; }
		bool operator== (const QTreeNode& other) const
		{ return this->index == other.index; }

		/* Operator construct for std::set<QTreeNode> */
		struct QTreeNodeLT {
			bool operator() (
				const std::unique_ptr<QTreeNode>& A,
				const std::unique_ptr<QTreeNode>& B) const
			{ return A->index < B->index; }
		};

		std::set<std::unique_ptr<QTreeNode>, struct QTreeNodeLT> Left;
		std::set<std::unique_ptr<QTreeNode>, struct QTreeNodeLT> Right;

		void Assign(T& input_data) { data = input_data; }
		bool Full() {
			if (this->LFull() && this->RFull()) return true;
			return false;
		}
		bool LFull() {
			if (Left.size()==2) return true;
			return false;
		}
		bool RFull() {
			if (Right.size()==2) return true;
			return false;
		}
		QTreeNode() : index(0), parent(nullptr), Left(), Right() {;}
		QTreeNode(const T& input_data, const uint64_t& input_index) : QTreeNode()
		{ data = input_data; index = input_index; }
		virtual ~QTreeNode() {;}
	};

private:
	uint64_t depth;
	uint64_t n_nodes;
	std::unique_ptr<QTreeNode> root_node;

	QTreeNode* search(const uint64_t& index);


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
