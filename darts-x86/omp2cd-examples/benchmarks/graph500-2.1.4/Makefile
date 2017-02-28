# -*- Makefile -*-
# Copyright 2010,  Georgia Institute of Technology, USA.
# See COPYING for license.
include make.inc
include ../../Makefile.def

CC=gcc
CFLAGS = -g -O3 -mtune=native -Wall -std=gnu99

GRAPH500_SOURCES=graph500.c options.c rmat.c kronecker.c verify.c prng.c \
	xalloc.c timer.c 

OMP2CD_SOURCES=graph500.c ./omp-csr/omp-csr.c

MAKE_EDGELIST_SOURCES=make-edgelist.c options.c rmat.c kronecker.c prng.c \
	xalloc.c timer.c 

BIN=seq-list/seq-list seq-csr/seq-csr make-edgelist

ifeq ($(BUILD_OPENMP), Yes)
BIN += omp
endif

ifeq ($(BUILD_XMT), Yes)
BIN = xmt-csr/xmt-csr xmt-csr-local/xmt-csr-local
endif

GENERATOR_SRCS=splittable_mrg.c	\
	graph_generator.c \
	make_graph.c utils.c

OMP_SOURCES=omp-csr/omp-csr.o $(GRAPH500_SOURCES:.c=.o) $(addprefix generator/,$(GENERATOR_SRCS:.c=.o))
OMP_TARGET=graph500.exe

.PHONY: all darts dartsTPLoops omp clean-darts clean

all: $(BIN)

CPPFLAGS += -I./generator

make-edgelist: CFLAGS:=$(CFLAGS) -I$(OPENMPINCLUDE) -fopenmp
make-edgelist:	make-edgelist.c options.c rmat.c kronecker.c prng.c \
	xalloc.c timer.c $(addprefix generator/,$(GENERATOR_SRCS))

seq-list/seq-list: seq-list/seq-list.c $(GRAPH500_SOURCES) \
	$(addprefix generator/,$(GENERATOR_SRCS))
seq-csr/seq-csr: seq-csr/seq-csr.c $(GRAPH500_SOURCES) \
	$(addprefix generator/,$(GENERATOR_SRCS))

xmt-csr/xmt-csr: CFLAGS:=$(CFLAGS) -pl xmt-csr/xmt-csr.pl
xmt-csr/xmt-csr: xmt-csr/xmt-csr.c $(GRAPH500_SOURCES) \
	$(addprefix generator/,$(GENERATOR_SRCS))

xmt-csr-local/xmt-csr-local: CFLAGS:=$(CFLAGS) -pl xmt-csr-local/xmt-csr-local.pl
xmt-csr-local/xmt-csr-local: xmt-csr-local/xmt-csr-local.c $(GRAPH500_SOURCES) \
	$(addprefix generator/,$(GENERATOR_SRCS))

generator/generator_test_seq: generator/generator_test_seq.c $(GENERATOR_SRCS)

generator/generator_test_omp: generator/generator_test_omp.c $(GENERATOR_SRCS)

omp: $(OMP_SOURCES)
	$(CC) -O3 $(OMP_SOURCES) -o $(OMP_TARGET) $(LDFLAGS) -L$(OPENMPLIB) -lomp -lm -lrt

%.o: %.c
	$(CC) $(CFLAGS) -I$(OPENMPINCLUDE) -fopenmp -c -o $@ $<

dartssimple:
	omp2cd $(OMP2CD_SOURCES)  -balanceOMPFor -IG=50 -inlineOMP -- -I$(OPENMPINCLUDE) -fopenmp $(CFLAGS)
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./omp-csr/*.output.darts.*
	
darts:
	omp2cd $(OMP2CD_SOURCES)  -balanceOMPFor -IG=50 -inlineOMP -useTPLoops -- -I$(OPENMPINCLUDE) -fopenmp $(CFLAGS)
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./omp-csr/*.output.darts.*

clean-darts:
	rm -f ./*.output.darts.* ./omp-csr/*.output.darts.*

clean:
	rm -f  *.o $(OMP_TARGET) ./omp-csr/*.o
	rm -f generator/generator_test_seq generator/generator_test_omp \
		$(BIN)
