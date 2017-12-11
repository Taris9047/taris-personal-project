# Select a proper gcc compiler (especially in Darwin environment)
SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
CC=gcc
CXX=g++
else ifneq (, $(findstring darwin, $(SYS)))
CC=gcc-7
CXX=g++-7
endif

LLVMCC=clang
LLVMFLAGS=-DNDEBUG -O3 -march=x86-64 -m64 -momit-leaf-frame-pointer

CFLAGS=-ggdb3
RELCFLAGS=-O3 -DNDEBUG -march=native -m64 -fomit-frame-pointer
LIBS=-pthread

PGCC=pgcc
PGCCFLAGS=-DNDEBUG -O4 -m64 -tp=x64

TARGET=data_process

OBJS=ip_addr.o
OBJS_REL=ip_addr_rel.o
OBJS_PGI=ip_addr_pgi.o
OBJS_LLVM=ip_addr_llvm.o

IP_ADDR_SOURCES=ip_addr.c ip_addr.h

OTHER_SOURCES=utils.h

all: debug


ip_addr.o: $(IP_ADDR_SOURCES) $(OTHER_SOURCES)
	$(CC) $(CFLAGS) -c ip_addr.c -o $@

ip_addr_rel.o: $(IP_ADDR_SOURCES) $(OTHER_SOURCES)
	$(CC) $(RELCFLAGS) -c ip_addr.c -o $@

ip_addr_pgi.o: $(IP_ADDR_SOURCES) $(OTHER_SOURCES)
	$(PGCC) $(PGCCFLAGS) -c ip_addr.c -o $@

ip_addr_llvm.o: $(IP_ADDR_SOURCES) $(OTHER_SOURCES)
	$(LLVMCC) $(RELCFLAGS) -c ip_addr.c -o $@


TARGET_SOURCES=data_process.c data_process.h

debug: $(TARGET_SOURCES) $(OTHER_SOURCES) $(OBJS)
	$(CC) $(CFLAGS) data_process.c -o $(TARGET) $(OBJS) $(LIBS)

release: $(TARGET_SOURCES) $(OTHER_SOURCES) $(OBJS_REL)
	$(CC) $(RELCFLAGS) data_process.c -o $(TARGET) $(OBJS_REL) $(LIBS)

pgi: $(TARGET_SOURCES) $(OTHER_SOURCES) $(OBJS_PGI)
	$(PGCC) $(PGCCFLAGS) data_process.c -o $(TARGET) $(OBJS_PGI) -lpthread

llvm: $(TARGET_SOURCES) $(OTHER_SOURCES) $(OBJS_LLVM)
	$(LLVMCC) $(LLVMFLAGS) data_process.c -o $(TARGET) $(OBJS_LLVM) $(LIBS)

clean:
	rm -rfv ./$(TARGET) *.dSYM ./*.o ./*.oo
