CC=gcc
CFLAGS=-ggdb3
LIBS=-pthread

TARGET=data_toss

all: debug
	
debug: data_toss.c data_toss.h
	$(CC) $(CFLAGS) data_toss.c -o data_toss $(LIBS)

release: data_toss.c data_toss.h
	$(CC) -O3 data_toss.c -o data_toss $(LIBS)

clean:
	rm -rfv ./$(TARGET) *.dSYM
