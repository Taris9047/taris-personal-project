/*****************************************

	Another attempt to implement
	a data structure with C++11 (STL)

	Trie

	Implementation file.

	Written by Taylor Shin
	July 12th 2017

******************************************/

#include "trie.hpp"

/*****************************************
  TrieNode definition
******************************************/
struct Trie::trie_node {
private:
  trie_data_t letter;
  std::vector<TrieNode> next_nodes;
  TrieNode parent;

public:
  /* Access methods */
  trie_data_t Data() const { return letter; }
  size_t NumNextNodes() const { return next_nodes.size(); }
  TrieNode Next(size_t index) const
  {
    if (!next_nodes.size()) return nullptr;
    if (index > next_nodes.size()-1) return nullptr;
    return next_nodes[index];
  }
  TrieNode Parent() const { return parent; }
  std::vector<TrieNode> NextNodes() const { return next_nodes; }

  /* Assignment methods */
  void SetData(char c) { letter = c; }
  void AddNext(const TrieNode& tn) { next_nodes.push_back(tn); }
  void SetParent(TrieNode p) { parent = p; }

  /* Constructors and destructors */
  trie_node() : letter('\0'), next_nodes(std::vector<TrieNode>()) {;}
  trie_node(const char c) : trie_node() { letter = c; }
  trie_node(const char c, TrieNode p) : trie_node(c) { parent = p; }
  /* copy constructor */
  trie_node(const trie_node& other) : trie_node()
  {
    letter = other.Data();
    next_nodes = std::vector<TrieNode>(other.NextNodes());
    parent = other.Parent();
  }
  ~trie_node() {;}
}; /* struct Trie::trie_node */


/*********************************************
  Trie class - Methods
**********************************************/
/* Insert string */
void Trie::Insert(std::string str)
{
  if (str.empty()) return;

  TrieNode tmp_tn, new_tmp_tn, curr_stage = nullptr;
  for (auto s=str.begin(); s!=str.end(); ++s) {

    if (s==str.begin()) {
      tmp_tn = search_roots(*s);
      if (!tmp_tn) {
        new_tmp_tn = new trie_node(*s);
        roots.push_back(new_tmp_tn);
      } /* f (s==str.begin()) */
      else new_tmp_tn = tmp_tn;
      curr_stage = new_tmp_tn;
      continue;
    } /* if (s==str.begin()) else */

    tmp_tn = search_nodes(*s, curr_stage->NextNodes());
    if (!tmp_tn) {
      new_tmp_tn = new trie_node(*s);
      new_tmp_tn->SetParent(curr_stage);
      curr_stage->NextNodes().push_back(new_tmp_tn);
    } /* if (!tmp_tn) */
    else {
      curr_stage = tmp_tn;
      continue;
    } /* if (!tmp_tn) else */

  } /* for (auto s=str.begin(); s!=str.end(); ++s) */
}
void Trie::Insert(const char* c_str)
{
  if (!c_str) return;
  Insert(std::string(c_str));
}

/* This string exists? */
bool Trie::Exists(std::string str)
{
  if (str.empty()) return false;

  std::vector<TrieNode> curr_nodes = roots;
  TrieNode found_node;
  for (auto s=str.begin(); s!=str.end(); ++s) {
    found_node = search_nodes(*s, curr_nodes);
    if (found_node) {
      curr_nodes = found_node->NextNodes();
    }
    else return false;
  }

  return true;
}
bool Trie::Exists(const char* c_str)
{
  if (!c_str) return false;
  return Exists(std::string(c_str));
}

/* Remove a string */
void Trie::Remove(std::string str)
{
  /* This may slow too much down our stuff. Let's not use it */
  //if (!Exists(str)) return;

  List* Stack = new List();
  std::vector<TrieNode> curr_nodes = roots;
  TrieNode found_node;
  for (auto s=str.begin(); s!=str.end(); ++s) {
    found_node = search_nodes(*s, curr_nodes);
    if (found_node) Stack->Push(found_node);
    else {
      delete Stack;
      return;
    }
  }

  /* Now we've made the list of nodes. Let's deal with it */
  TrieNode tmp_parent, tmp;
  size_t tmp_v_i;
  for (auto i=0; i<Stack->Len(); ++i) {
    tmp = (TrieNode)Stack->At(i);
    tmp_parent = tmp->Parent();
    erase_vec_elem<TrieNode>(tmp_parent->NextNodes(), tmp);
    if (tmp_parent->NextNodes().empty())
      delete tmp;
    tmp = tmp_parent;
  }

  delete Stack;

}
void Trie::Remove(const char* c_str)
{
  if (!c_str) return;
  Remove(std::string(c_str));
}


/* Returns number of roots */
size_t Trie::NumRoots() const
{
  return roots.size();
}

/* Get vector of roots */
std::vector<Trie::TrieNode> Trie::GetRoots() const
{
  return roots;
}

/*********************************************
  Trie class - Private methods
**********************************************/
/* Delete a trie branch */
void Trie::delete_trie_branch(TrieNode root)
{
  if (!root) return;

  List* Stack = new List();
  TrieNode tmp_trn;

  Stack->Push(root);
  while (!Stack->IsEmpty()) {

    tmp_trn = (TrieNode)Stack->Pop();

    if (!tmp_trn) break;

    if (tmp_trn->NumNextNodes()>0) {
      for (auto i=tmp_trn->NextNodes().begin();
        i!=tmp_trn->NextNodes().end(); ++i)
        Stack->Push(*i);
    } /* if (tmp_trn->NumNextNodes()>0) */

    delete tmp_trn;

  } /* while (!Stack->IsEmpty()) */

  delete Stack;
}

/* Search roots with a character */
Trie::TrieNode Trie::search_roots(const char c)
{
  return search_nodes(c, roots);
}

/* Search child nodes */
Trie::TrieNode Trie::search_nodes(const char c, std::vector<TrieNode> nodes)
{
  if (c=='\0') return nullptr;
  if (nodes.size()==0) return nullptr;

  Trie::TrieNode found_node = nullptr;
  #pragma omp parallel shared(found_node)
  for (auto ni=nodes.begin(); ni!=nodes.end(); ++ni) {
    #pragma omp single nowait
    {
      // if (found) continue;
      if ((*ni)->Data() == c) found_node = *ni;
    } /* #pragma omp single nowait */
  }

  return found_node;
}


/*********************************************
  Trie class - Constructors and Destructors
**********************************************/
Trie::Trie() : \
  roots(std::vector<TrieNode>())
{}

Trie::Trie(const Trie& other) : Trie()
{
  roots = other.GetRoots();
}

Trie::~Trie()
{
  #pragma omp parallel
  for (auto i=roots.begin(); i!=roots.end(); ++i) {
    #pragma omp single nowait
    {
      delete_trie_branch(*i);
    }
  } /* for (auto i=roots.begin(); i!=roots.end(); ++i) */

}
