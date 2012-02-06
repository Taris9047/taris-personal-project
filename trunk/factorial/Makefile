CC=gcc
FCC=gfortran
CFLAGS=-O3 -Wall
FFLAGS=-O3 -Wall
F90FLAGS=-O3 -Wall -ffree-form

all:fact_c fact_f77

fact_c: factorial.c
	$(CC) $(CFLAGS) -o ./fact_c ./factorial.c -lgmp

mpfun77.o: ./mpfun/mpfun77/mpfun.f
	$(FCC) $(FFLAGS) -o mpfun77.o -c ./mpfun/mpfun77/mpfun.f 

transmp77: ./mpfun/mpfun77/transmp.f
	$(FCC) $(FFLAGS) -o transmp77 ./mpfun/mpfun77/transmp.f

mpfun90.o: ./mpfun/mpfun90/f90/mpfun90.f
	cd ./mpfun/mpfun90/f90/;make

factorial-mpfun77.f: factorial.f transmp77
	./transmp77 < factorial.f > factorial-mpfun77.f

fact_f77: mpfun77.o factorial-mpfun77.f
	$(FCC) $(FFLAGS) -o ./fact_f77 ./factorial-mpfun77.f ./mpfun77.o

clean:
	rm -rf fact_c fact_f77 mpfun77.o mpfun90.o transmp77 factorial-mpfun77.f
	cd ./mpfun/mpfun90/f90/;make clean

run: fact_f77 fact_c
	time ./fact_c
	time ./fact_f77