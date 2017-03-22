CC=clang

CFLAGS=-ggdb3 -Wall -std=c11
LIBS=-lm -L.

SOURCES=$(wildcard *.c *.C *.m)
HEADERS=$(wildcard *.h)
OBJS=$(SOURCES:.c=.o)

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c $(OBJS)
	$(CC) $(CFLAGS) $< -o dict_test

all: test

clean:
	rm -rfv *.o dict_test
