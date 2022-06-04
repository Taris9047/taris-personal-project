CC=gcc
CFLAGS_DBG=-g3 -ggdb -W -Wall
CFLAGS_OPT=-O3 -march=native -fomit-frame-pointer -pipe -W -Wall
LIBS=-lm

all: test_opt test_dbg

Q_rsqrt_dbg.o: Q_rsqrt.c Q_rsqrt.h
	$(CC) $(CFLAGS_DBG) -c $< -o $@

Q_rsqrt_opt.o: Q_rsqrt.c Q_rsqrt.h
	$(CC) $(CFLAGS_OPT) -c $< -o $@

test_opt: test.c Q_rsqrt_opt.o
	$(CC) $^ $(CFLAGS_OPT) $(LIBS) -o $@

test_dbg: test.c Q_rsqrt_dbg.o
	$(CC) $^ $(CFLAGS_DBG) $(LIBS) -o $@

clean:
	rm -rfv ./*.o ./*.Po ./test_dbg ./test_opt