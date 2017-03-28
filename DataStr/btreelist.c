/***********************************************

 Binary Tree with List

 Implementation file

 Written by Taylor Shin

 March. 28th 2017

************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "btreelist.h"

/***********************************************
 Constructors and Destructors
************************************************/
BTreeList NewBTreeList()
{
	BTreeList btl = (BTreeList)malloc(sizeof(binary_tree_list));
	assert(btl);

	btl->data_tree = NewBTree();
	btl->element_num_tree = NewBTree();
	btl->n_lists = 0;

	return btl;
}

int DeleteBTreeList(BTreeList btl)
{
	assert(btl);

	DeleteBTreeHard(btl->data_tree, DeleteList);
	DeleteBTreeHard(btl->element_num_tree, NULL);
	btl->n_lists = 0;

	return 0;
}


/***********************************************
 Methods
************************************************/
int BTLInsert(BTreeList btl, btree_data_t data, btree_key_t key)
{
	assert(btl);

	List found_list = \
		(List)BTSearch(btl->data_tree, key);
	unsigned long long* found_list_len = \
		(unsigned long long*)BTSearch(btl->element_num_tree, key);

	if (!found_list && !found_list_len) {
		BTInsert(btl->data_tree, data, key);
		found_list_len = (unsigned long long*)malloc(sizeof(unsigned long long));
		(*found_list_len) = 1;
		BTInsert(btl->element_num_tree, found_list_len, key);
	}
	else if (found_list && found_list_len) {
		LPush(found_list, data);
		(*found_list_len)++;
	}

	return 0;
}

bool BTLSearch(BTreeList btl, btree_key_t key, List* found_list)
{
	assert(btl);

	(*found_list) = (List)BTSearch(btl->data_tree, key);

	if (*found_list) return true;
	else return false;
}
