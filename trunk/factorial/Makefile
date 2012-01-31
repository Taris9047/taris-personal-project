CC=gcc
FCC=gfortran
CFLAGS=-O3 -Wall
FFLAGS=-O3 -Wall

all:fact_c fact_f77

fact_c: factorial.c
	$(CC) $(CFLAGS) -o ./fact_c ./factorial.c -lgmp

fact_f77: factorial.f
	$(FCC) $(FFLAGS) -o ./fact_f77 ./factorial.f

clean:
	rm -rf fact_c fact_f77