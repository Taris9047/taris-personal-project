CC=gcc
CFLAGS=-ggdb3 -fPIC
LIBS=-lm

TESTF_NAME=HashTest

OBJS=hash.o dict.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS) test.c
	$(CC) $(CFLAGS) $< test.c -o $(TESTF_NAME) $(LIBS)

clean:
	rm -rfv *.o
	rm -rfv ./$(TESTF_NAME)
