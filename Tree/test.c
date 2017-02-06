/***************************************

  Practice coding for binary tree
  
  Test file

  Taylor Shin, Feb. 05 2017

 ***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bintree.h"

int main(int argc, char* argv[])
{
    printf("Test program for Binary tree.\n");
    
    BINode* Root = InitBITree();

    FreeBITree(Root);
    
    return 0;
}
