/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Octal tree

  header file

**********************************************/
#ifndef CPP11_IMPLEMENTATION_DATASTR_OCTAL_TREE_HEADER
#define CPP11_IMPLEMENTATION_DATASTR_OCTAL_TREE_HEADER

template<class T>
class OCTree {

protected:

	struct OCTreeNode {

		struct OCTreeNode* parent;

		T data;
		uint64_t index;

		bool operator< (const OCTreeNode& other) const
		{ return this->index < other.index; }
		bool operator> (const OCTreeNode& other) const
		{ return this->index > other.index; }
		bool operator== (const OCTreeNode& other) const
		{ return this->index == other.index; }

		/* Operator construct for std::set<QTreeNode> */
		struct OCTreeNodeLT {
			bool operator() (
				const std::unique_ptr<OCTreeNode>& A,
				const std::unique_ptr<OCTreeNode>& B)
			{ return A->index < B->index; }
		};

		std::set<std::unique_ptr<OCTreeNode>, OCTreeNodeLT> Left;
		std::set<std::unique_ptr<OCTreeNode>, OCTreeNodeLT> Right;

		void Assign(const T& input_data) { data = input_data; }
		bool Full() const {
			if (this->LFull() && this->RFull()) return true;
			return false;
		}
		bool LFull() const {
			if (Left.size()==4) return true;
			return false;
		}
		bool RFull() const {
			if (Right.size()==4) return true;
			return false;
		}
		uint64_t Index() const { return index; }
		OCTreeNode() : index(0), parent(nullptr), Left(), Right() {;}
		OCTreeNode(const T& input_data, const uint64_t& input_index) : OCTreeNode()
		{ data = input_data; index = input_index; }
		virtual ~OCTreeNode() {;}
	};

private:
	uint64_t depth;
	uint64_t n_nodes;
	std::unique_ptr<OCTreeNode> root_node;

	OCTreeNode* search(const uint64_t& index);


public:
	/* Methods */
	void Insert(const T& data, const uint64_t& index);
	T& Get(const uint64_t& index);

	/* Constructors and Destructors */
	OCTree();
	virtual ~OCTree();

}; /* class OCTree */



#include "octree.tcc"

#endif /* Include guard */
