#include <stdio.h>
#include <gmp.h>
#include <time.h>
 
void factorial(mpz_t* f, unsigned int n)
{
    unsigned int i = 1;
 
    for (i = 1; i <= n ; i++){
        mpz_mul_si(*f, *f, i);
    }
}
 
int main(int argc, char **argv)
{
    mpz_t facN;
    unsigned int n = 50000;
    clock_t start, end;
    double runtime;
 
    mpz_init(facN);
    mpz_set_ui(facN, 1L);
 
    start = clock();
    factorial(&facN, n);
    end = clock();
 
    //printf("fac %u is ", n);
    //mpz_out_str(stdout, 10, facN);
    //printf("\n");
 
    runtime = (double)(end-start) / CLOCKS_PER_SEC;
    printf ("runtime is %f\n", runtime);
    mpz_clear(facN);

	return 0;
}