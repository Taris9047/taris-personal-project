/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Trie

  header file

**********************************************/
#ifndef CPP11_IMPLEMENTATION_DATASTR_TRIE_HEADER
#define CPP11_IMPLEMENTATION_DATASTR_TRIE_HEADER

#include <algorithm>
#include <list>
#include <memory>
#include <vector>
#include <string>

template <class T>
class Trie {

protected:
	struct TrieNode {
	private:
		T data;
		TrieNode* parent;
		std::list<std::unique_ptr<TrieNode>> children;

	public:
		T GetData() const { return data; }
		TrieNode* GetParent() const { return parent; }
		int nChildren() { return children.size(); }

		std::vector<TrieNode*> GetChildren();
		TrieNode* PopChildren();
		void PushChildren(const TrieNode* tn);
		TrieNode* GetChildrenFront();
		TrieNode* GetChildrenBack();

		TrieNode() : data(), parent(nullptr), children() {;}
		TrieNode(const T& inp_data) : TrieNode() { data = inp_data; }
		TrieNode(const T& inp_data, const TrieNode*& inp_parent) : TrieNode(inp_data) { parent = inp_parent; }

		virtual ~TrieNode() {;}
	};

private:

	std::unique_ptr<TrieNode> root_node;
	uint64_t depth;
	uint64_t n_data;

	TrieNode* traverse(const std::vector<T>& path_bf);

public:

	void Insert(const std::vector<T>& new_path);
	std::vector<std::vector<T>> Paths(const std::vector<T>& path_bf);
	std::vector<T> NextNodes(const std::vector<T>& path_bf);

	uint64_t Depth() const { return depth; }
	uint64_t nData() const { return n_data; }

	Trie() : root_node(nullptr), depth(0), n_data(0) {;}
	Trie(const std::vector<T>& data_list);
	// Trie(const char* c_str);
	// Trie(std::string str);
	virtual ~Trie() {;}
};


#include "trie.tcc"

#endif /* Include guard */
