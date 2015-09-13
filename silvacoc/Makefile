CC=gcc
CFLAGS=-O2 -Wall -fPIC

all: silvacoc
	./silvacoc sample_deck.in

silvacoc: silvacoc.c parsor.o
	$(CC) $(CFLAGS) silvacoc.c -o $@ -lm

parsor.o: parsor.c
	$(CC) $(CFLAGS) -c parsor.c

clean:
	rm -rf *.o silvacoc silvacoc.exe