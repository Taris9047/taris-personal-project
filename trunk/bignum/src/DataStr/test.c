#include "llist.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	printf ("Linked List Test\n");

	List* s_test_lst = SLalloc(10);
	printf("List[%llu] found\n", s_test_lst->index);
	printf("List[%llu] found\n", s_test_lst->nextList->index);
	printf("List[%llu] found\n", SLfind(s_test_lst, 8)->index);
	printf("Length of s_test_lst = %llu\n", SLlen(s_test_lst));

	List* s_string_lst = SLstralloc("Hello, World!!\n");
	ULLONG i;
	for (i = 0; i < SLlen(s_string_lst); i++) {
		//printf("%c", *(char*)SLfind(s_string_lst, i)->content);
		printf("%c", SLread(s_string_lst, i, char));
	}

	char arr[3] = "arr";
	SLset(s_string_lst, 1, arr[0]);
	SLset(s_string_lst, 2, arr[1]);
	SLset(s_string_lst, 3, arr[2]);

	for (i = 0; i < SLlen(s_string_lst); i++) {
		printf("%c", SLread(s_string_lst, i, char));
	}

	SLfree(s_test_lst);
	SLfree(s_string_lst);

	return 0;
}