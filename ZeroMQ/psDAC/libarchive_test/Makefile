CC=clang
CFLAGS=-ggdb3
LIBS=-larchive

all: libarchive_test

libarchive_test: libarchive_test.c
	$(CC) $(CFLAGS) $< -o libarchive_test $(LIBS)

clean:
	rm -rfv libarchive_test