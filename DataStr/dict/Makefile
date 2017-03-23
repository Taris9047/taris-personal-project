CC=clang

CFLAGS=-ggdb3 -Wall -std=c1x
LIBS=-lm -L.

SOURCES=$(filter-out test.c, $(wildcard *.c *.C *.m))
HEADERS=$(filter-out test.h, $(wildcard *.h))
OBJS=$(SOURCES:.c=.o)

.c.o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c $(OBJS)
	$(CC) $(CFLAGS) test.c $(OBJS) -o dict_test

all: test

clean:
	rm -rfv *.o dict_test
