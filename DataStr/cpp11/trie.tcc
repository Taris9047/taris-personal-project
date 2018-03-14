/*********************************************

  C++ 11 implementation of some useful data
  structures.

  Trie

  Implementation file

**********************************************/

/*********************************************
  Trie Node stuffs
**********************************************/
template <class T>
std::vector<typename Trie<T>::TrieNode*> Trie<T>::TrieNode::GetChildren()
{
	if (!this->nChildren()) return std::vector<TrieNode*>();

	std::vector<TrieNode*> nodes(this->nChildren());
	uint64_t ind = 0;
	for (auto c : children) {
		nodes[ind] = c.get();
		ind++;
	}
	return nodes;
}

template <class T>
typename Trie<T>::TrieNode* Trie<T>::TrieNode::PopChildren()
{
	auto c = children.front().get();
	children.pop_front();
	return c;
}

template <class T>
void Trie<T>::TrieNode::PushChildren(const TrieNode* tn)
{
	if (!tn) return; /* Do nothing if nullptr comes in */
	children.push_back(std::make_unique<TrieNode>(tn));
	return;
}

template <class T>
typename Trie<T>::TrieNode* Trie<T>::TrieNode::GetChildrenFront()
{
	if (children.empty()) return nullptr;
	return children.front().get();
}

template <class T>
typename Trie<T>::TrieNode* Trie<T>::TrieNode::GetChildrenBack()
{
	if (children.empty()) return nullptr;
	return children.back().get();
}

/*********************************************
  Trie stuffs - Methods
**********************************************/
/* Private - Traverse with given data set */
template <class T>
typename Trie<T>::TrieNode* Trie<T>::traverse(const std::vector<T>& path_bf)
{
	if (!path_bf.size()) return nullptr;

	auto tmp_ptr = root_node.get();
	for (auto p : path_bf) {
		if (tmp_ptr->GetData() != p) break;
		if (tmp_ptr->nChildren() == 0) break;

		for (auto c : tmp_ptr->GetChildren()) {
			if (c->GetData() == p) {
				tmp_ptr = c;
				continue;
			}
		}
		/* We couldn't find a perfect match but this would do */
		break;
	} /* for (auto p : path_bf) */

	return tmp_ptr;
}


/* Insert stuff */
template <class T>
void Trie<T>::Insert(const std::vector<T>& new_path)
{
	/* Weird input: Just ignore this crap */
	if (new_path.empty()) return;

	auto tmp_ptr = root_node.get();
	for (auto i=0; i<new_path.size(); ++i) {
		if (new_path[i] != tmp_ptr->GetData()) break;
		if (tmp_ptr->nChildren() == 0) break;
		if (i==new_path.size()-1) break;

		for (auto c : tmp_ptr->GetChildren()) {
			if (c->GetData() == new_path[i+1]) tmp_ptr = c;
			else {
				auto new_node = new TrieNode(new_path[i+1], tmp_ptr);
				tmp_ptr->PushChildren(new_node);
				tmp_ptr = tmp_ptr->GetChildrenBack().get();
			}
			continue;
		} /* for (auto c : tmp_ptr->GetChildren()) */
	} /* for (auto i=0; i<new_path.size(); ++i) */

	return;
}

/* Getting possible paths */
template <class T>
std::vector<std::vector<T>> Trie<T>::Paths(const std::vector<T>& path_bf)
{
	auto trav_node = traverse(path_bf);
	auto next_nodes = trav_node->GetChildren();

	std::vector<std::vector<T>> ret_vec(next_nodes.size());

	// Gotta think about some way to collect all the possible branches.


}

/* Getting heads of possible data */
template <class T>
std::vector<T> Trie<T>::NextNodes(const std::vector<T>& path_bf)
{
	auto current_node = traverse(path_bf);

	if (current_node == nullptr) return std::vector<T>();
	if (current_node->children.size() == 0) return std::vector<T>();

	auto nodes = current_node->GetChildren();

	std::vector<T> ret_data(current_node->nChildren());
	for (auto i=0; i<nodes.size(); ++i) ret_data[i] = nodes[i].GetData();

	return ret_data;
}

/*********************************************
  Trie stuffs - Constrcutors and Destructors
**********************************************/
template <class T>
Trie<T>::Trie(const std::vector<T>& data_list) : Trie()
{
	Insert(data_list);
}


// template <class T>
// Trie<T>::Trie(const char* cstr)
// {
//
// }
//
// template <class T>
// Trie<T>::Trie(std::string str)
// {
//
// }
