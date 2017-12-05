# Select a proper gcc compiler (especially in Darwin environment)
SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
CC=gcc
CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
CC=gcc-7
CXX=g++-7
endif

CFLAGS=-ggdb3
RELCFLAGS=-DNDEBUG -O3 -march=native -m64 -fomit-frame-pointer
RELLLVMFLAGS=-DNDEBUG -O3 -march=x86-64 -m64 -momit-leaf-frame-pointer
LIBS=-pthread

LLVMCC=clang

PGCC=pgcc
PGCCFLAGS=-DNDEBUG -m64 -tp=x64 -Mvect -Mcache_align -Mipa=fast,inline -Munroll=c:65535

MPICC=mpicc
MPICFLAGS=-DUSE_MPI

OBJS=rand_safe.o
OBJS_REL=rand_safe_rel.o
OBJS_PGI=rand_safe_pgi.o
OBJS_LLVM_REL=rand_safe_rel_llvm.o
TARGET=data_toss

all: debug

rand_safe.o: rand_safe.c rand_safe.h
	$(CC) $(CFLAGS) -c rand_safe.c -o $@

rand_safe_rel.o: rand_safe.c rand_safe.h
	$(CC) $(RELCFLAGS) -c rand_safe.c -o $@

rand_safe_rel_llvm.o: rand_safe.c rand_safe.h
	$(LLVMCC) $(RELLLVMFLAGS) -c rand_safe.c -o $@

rand_safe_pgi.o: rand_safe.c rand_safe.h
	$(PGCC) $(PGCCFLAGS) -c rand_safe.c -o $@

debug: data_toss.c data_toss.h $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) data_toss.c -o $(TARGET) $(LIBS)

release: data_toss.c data_toss.h $(OBJS_REL)
	$(CC) $(RELCFLAGS) $(OBJS_REL) data_toss.c -o $(TARGET) $(LIBS)

llvm: data_toss.c data_toss.h $(OBJS_LLVM_REL)
	$(LLVMCC) $(RELLLVMFLAGS) $(OBJS_LLVM_REL) data_toss.c -o $(TARGET) $(LIBS)

pgi: data_toss.c data_toss.h $(OBJS_PGI)
	$(PGCC) $(PGCCFLAGS) $(OBJS_PGI) data_toss.c -o $(TARGET) -lpthread

mpi: data_toss.c data_toss.h $(OBJS_REL)
	$(MPICC) $(MPICFLAGS) $(RELCFLAGS) $(OBJS_REL) data_toss.c -o $(TARGET) -lpthread

clean:
	rm -rfv ./$(TARGET) *.dSYM ./*.o ./*.oo
	rm -rfv vcore.*
