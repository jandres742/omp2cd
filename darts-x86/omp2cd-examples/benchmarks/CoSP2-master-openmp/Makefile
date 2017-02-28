# Makefile for CoSP2 serial and parallel versions
#
include ../../Makefile.def

SHELL = /bin/bash

# double precision (ON/OFF)
DOUBLE_PRECISION = ON
# MPI for parallel (ON/OFF)
#DO_MPI = ON
DO_MPI = OFF

# Contiguous arrays
CONTIG_MATRIX = ON
#CONTIG_MATRIX = OFF

# Column position for diagonal elements in row
DIAGPOS = POS1
#DIAGPOS = POSJ

CC = gcc
#CC = mpicc
CFLAGS = -std=c99 -I$(OPENMPINCLUDE) -fopenmp -D$(DIAGPOS)
OPTFLAGS = -O3
INCLUDES =
C_LIB = -lm

MPI_LIB =
MPI_INCLUDE =

OTHER_LIB =
OTHER_INCLUDE =

# clear all suffixes
.SUFFIXES:
# list only those that we use
.SUFFIXES: .c .o

.PHONY: DEFAULT clean distclean darts omp clean-darts

BIN_DIR=.

# Add contiguous arrays
ifeq ($(CONTIG_MATRIX), ON)
CFLAGS += -DCONTIG_MATRIX
endif

# Check for double precision
ifeq ($(DOUBLE_PRECISION), ON)
CFLAGS += -DDOUBLE
else
CFLAGS += -DSINGLE
endif

# Set executable name and add includes & libraries for MPI if needed.
# CoSP2_VARIANT = CoSP2-serial
# ifeq ($(DO_MPI), ON)
CoSP2_VARIANT = cosp2
# INCLUDES += ${MPI_INCLUDE}
# CFLAGS += -DDO_MPI
# LDFLAGS += ${MPI_LIB}
# endif
CoSP2_EXE = ${BIN_DIR}/${CoSP2_VARIANT}.exe

LDFLAGS += ${C_LIB} ${OTHER_LIB} -L$(OPENMPLIB) -lomp
CFLAGS  += ${OPTFLAGS} ${INCLUDES} ${OTHER_INCLUDE}

#This is to use dynamically allocated arrays instead of static arrays for DARTS
#-DSTATIC_ARRAYS=1 for OpenMP
#-DSTATIC_ARRAYS=0 for DARTS

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

DEFAULT: ${CoSP2_EXE}

%.o: %.c
	${CC} ${CFLAGS} -DSTATIC_ARRAYS=1 -c $< -o $@

${CoSP2_EXE}: ${BIN_DIR} ${OBJECTS}
	${CC} -O3 -o ${CoSP2_EXE} ${OBJECTS} ${LDFLAGS}

${BIN_DIR}:
	@if [ ! -d ${BIN_DIR} ]; then mkdir -p ${BIN_DIR} ; fi

darts:
	omp2cd $(OMP2CDFLAGS) $(SOURCES) -- $(CLANGFLAGS) -DSTATIC_ARRAYS=0
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

clean-darts:
	rm -f ./*.output.darts.*

clean:
	rm -f *.o ${CoSP2_EXE}

distclean:
	rm -f ${CoSP2_EXE}
	rm -rf html latex

-include .depend
