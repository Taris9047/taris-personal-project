CC=gcc
FCC=gfortran
CFLAGS=-O3 -Wall
FFLAGS=-O3 -Wall
F90FLAGS=-O3 -Wall -ffree-form
F90INCLUDE=./mpfun/mpfun90/f90/

all:fact_c fact_f77 fact_f90

fact_c: factorial.c
	$(CC) $(CFLAGS) -o $@ ./factorial.c -lgmp

mpfun77.o: ./mpfun/mpfun77/mpfun.f
	$(FCC) $(FFLAGS) -o $@ -c ./mpfun/mpfun77/mpfun.f 

transmp77: ./mpfun/mpfun77/transmp.f
	$(FCC) $(FFLAGS) -o $@ ./mpfun/mpfun77/transmp.f

factorial-mpfun77.f: factorial.f transmp77
	./transmp77 < factorial.f > $@

fact_f77: mpfun77.o factorial-mpfun77.f
	$(FCC) $(FFLAGS) -o $@ ./factorial-mpfun77.f ./mpfun77.o

mpfun90.o: $(F90INCLUDE)mpfun90.f
	$(FCC) $(F90FLAGS) -c -o $@ $(F90INCLUDE)mpfun90.f

mpmod90.o: $(F90INCLUDE)mpmod90.f
	$(FCC) $(F90FLAGS) -c -o $@ $(F90INCLUDE)mpmod90.f

mpmodm90.o: $(F90INCLUDE)mpmodm90.f
	$(FCC) $(F90FLAGS) -c -o $@ $(F90INCLUDE)mpmodm90.f

mpmodx90.o: $(F90INCLUDE)mpmodx90.f
	$(FCC) $(F90FLAGS) -c -o $@ $(F90INCLUDE)mpmodx90.f

fact_f90: factorial.f90 \
	mpfun90.o mpmod90.o mpmodm90.o mpmodx90.o
	$(FCC) $(F90FLAGS) -o $@ ./factorial.f90 \
	mpfun90.o mpmod90.o mpmodm90.o mpmodx90.o \
	-I$(F90INCLUDE)

clean:
	rm -rf ./fact_c 
	rm -rf ./fact_f77 ./transmp77 ./factorial-mpfun77.f
	rm -rf ./fact_f90
	rm -rf ./*.o ./*.mod
	cd ./mpfun/mpfun90/f90/;make clean

run: fact_f77 fact_c
	time ./fact_c
	time ./fact_f77
	time ./fact_f90