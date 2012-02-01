CC=gcc
FCC=gfortran
CFLAGS=-O3 -Wall
FFLAGS=-O3 -Wall

all:fact_c fact_f77

fact_c: factorial.c
	$(CC) $(CFLAGS) -o ./fact_c ./factorial.c -lgmp

mpfun77.o: ./mpfun/mpfun77/mpfun.f
	$(FCC) $(CFLAGS) -o mpfun77.o -c ./mpfun/mpfun77/mpfun.f 

mpfun90.o: ./mpfun/mpfun90/mpfun.f90
	$(FCC) $(CFLAGS) -o mpfun90.o -c ./mpfun/mpfun90/mpfun.f90 

fact_f77: mpfun77.o factorial.f
	$(FCC) $(FFLAGS) -o ./fact_f77 ./factorial.f

clean:
	rm -rf fact_c fact_f77 mpfun77.o mpfun90.o