/**
* @Author: taris
* @Date:   2017-03-04T21:14:28-06:00
* @Last modified by:   taris
* @Last modified time: 2017-03-04T22:09:22-06:00
*/



/************************************

 Test program for bignum

 Written by Taylor Shin
 March 4th 2017

*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bignum.h"

/* Some default numbers and operations */
const char* DEF_NUMA = "100";
const char* DEF_NUMB = "500";
#define DEF_OP ADD

/* Check operation */
static enum oper {ADD, SUB, MUL, DIV, REM} _oper;
static const char add_op[] = "add";
static const char sub_op[] = "sub";
static const char mul_op[] = "mul";
static const char div_op[] = "div";
static const char rem_op[] = "rem";

static enum oper check_op(const char* op_str)
{
    assert(op_str);

    int rc;

    rc = strcmp(add_op, op_str);
    if (!rc) return ADD;

    rc = strcmp(sub_op, op_str);
    if (!rc) return SUB;

    rc = strcmp(mul_op, op_str);
    if (!rc) return MUL;

    rc = strcmp(div_op, op_str);
    if (!rc) return DIV;

    rc = strcmp(rem_op, op_str);
    if (!rc) return REM;
}


/* The main function */
int main(int argc, char* argv[])
{
    const char* numA;
    const char* numB;
    enum oper op;

    if (argc == 4) {
        numA = argv[1];
        numB = argv[2];
        op = check_op(argv[3]);
    }
    else {
        numA = DEF_NUMA;
        numB = DEF_NUMB;
        op = DEF_OP;
    }




    return 0;
}
