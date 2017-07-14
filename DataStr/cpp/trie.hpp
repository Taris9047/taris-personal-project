/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Trie

	Header file.

	Written by Taylor Shin
	July 12th 2017

******************************************/

#ifndef CPP11_DATA_STRUCTURE_IMPLEMENTATION_TRIE_HEADER
#define CPP11_DATA_STRUCTURE_IMPLEMENTATION_TRIE_HEADER

#if defined(_OPENMP)
#  include <omp.h>
#endif

#include "utils.hpp"
#include "list.hpp"

#include <memory>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstring>

typedef char trie_data_t;

/* The Trie class */
class Trie {
public:
  /* Node struct definition */
  struct trie_node;
  typedef trie_node* TrieNode;

  /* Methods */
  void Insert(std::string str);
  void Insert(const char* c_str);
  bool Exists(std::string str);
  bool Exists(const char* c_str);
  void Remove(std::string str);
  void Remove(const char* c_str);

  size_t NumRoots() const;
  std::vector<TrieNode> GetRoots() const;

  /* Constructors and destructors */
  Trie();
  Trie(const Trie&); /* copy constructor */
  Trie& operator= (const Trie&); /* copy operator */
  ~Trie();

private:
  /* vector of staring TrieNodes */
  std::vector<TrieNode> roots;

  /* Delete a trie branch */
  void delete_trie_branch(TrieNode);

  /* Search roots with a character */
  TrieNode search_roots(const char c);
  TrieNode search_nodes(const char c, std::vector<TrieNode> nodes);


}; /* Trie class */

#endif /* Include guard */
