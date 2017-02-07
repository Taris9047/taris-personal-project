CC=gcc
CFLAGS=-ggdb3 -fPIC
LIBS=-lm

TESTF_NAME=BITreeTest

OBJS=bintree.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS) test.c
	$(CC) $(CFLAGS) $< test.c -o $(TESTF_NAME) $(LIBS)

clean:
	rm -rfv *.o
	rm -rfv ./$(TESTF_NAME)
