#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

BNI factorial(BNI n);

int main(int argc, char* argv[])
{
    BNI numA;
    BNI numB;

    if (argc == 3) {
        numA = BNICC(argv[1]);
        numB = BNICC(argv[2]);
    }
    else if (argc == 2) {
        numA = BNICC(argv[1]);
        numB = BNICC("0");
    }
    else {
        numA = BNICC("11");
        numB = BNICC("10");
    }
    BNI result = BNII(0);

    printf("numA: %s\n", BNItostr(numA));
    printf("numB: %s\n", BNItostr(numB));

    BNIadd(result, numA, numB);

    printf("%s + %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(result));

    result = BNII(0);
    BNIsub(result, numA, numB);

    printf("%s - %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(result));

    printf("\n");
    printf("Performing BNImul\n");
    BNI answermul = BNII(0);
    BNImul(answermul, numA, numB);
    printf("%s * %s = %s\n", BNItostr(numA), BNItostr(numB), BNItostr(answermul));
    printf("\n");
    printf("Performing factorial!!\n");
    printf("%s! = %s\n", BNItostr(numA), BNItostr(factorial(numA)));

    printf("\n");
    printf("Press enter key to finish\n");
    getchar();

    return 0;
}


BNI factorial(BNI n)
{
    BNI factN = BNII(1);
    BNI Tmp = BNII(0);
    BNI TmpIndex = BNII(0);
    BNI Index = BNII(1);
    BNI one = BNII(1);
    if (n->sign == FALSE)
        return factN;

    if (BNIeq(n, BNII(0)))
        return factN;

    char* index;
    do {
        //BNIfree(Tmp); BNIfree(TmpIndex);
        BNIadd(TmpIndex, Index, one);
        BNIcpy(Index, TmpIndex); TmpIndex = BNII(0);
        index = BNItostr(Index);
        BNImul(Tmp, factN, Index);
        BNIcpy(factN, Tmp); Tmp = BNII(0);
        printf("factorial, %s! = %s\n", index, BNItostr(factN));
    } while(!BNIeq(n, Index));

    return factN;
}
