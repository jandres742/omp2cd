## Makefile for SCF-C
include ../../Makefile.def

#CC = gcc 
CFLAGS += -g -O3 -mtune=native -Wall -std=gnu99
LDFLAGS += -lm

OBJFILES = input.o integ.o output.o timer.o scf.o diagonalize.o twoel.o
MTARGET = scf.exe

SOURCES=$(OBJFILES:.o=.c)

# ifdef BLAS
CFLAGS += -DBLAS
# LDFLAGS += -lblas
LDFLAGS += $(LDFLAGS_ATLAS)
# endif

ifdef NO_PRECALC
CFLAGS += -DNO_PRECALC
endif

ifdef USE_CACHE
CFLAGS += -DUSE_CACHE
endif

ifndef NO_BOOKKEEPING
CFLAGS += -DBOOKKEEPING
endif

ifdef BAD_CACHE
CFLAGS += -DBAD_CACHE_UTILIZATION
endif

ifdef LAPACKE
CFLAGS += -DUSE_LAPACK
LDFLAGS += -llapacke -llapack -lblas
endif

ifeq (x$(MKLROOT),x)
else
CFLAGS += -I$(MKLROOT)/include -DUSE_MKL -DUSE_LAPACK -DBLAS
LDFLAGS += -L$(MKLROOT)/lib/intel64 -L$(MKLROOT)/../compiler/lib/intel64 -Wl,--start-group -lmkl_intel_lp64  -lmkl_intel_thread -lmkl_core -Wl,--end-group
endif

ifndef OMP_VERSION
CFLAGS += -DOMP_VERSION=3
endif

.PHONY: all darts omp clean-darts clean

all: $(MTARGET)

scf.exe:  $(OBJFILES)
	$(CC) -O3 $(OBJFILES) -o $(MTARGET) $(LDFLAGS) -L$(OPENMPLIB) -lomp

%.o: %.c
	$(CC) $(CFLAGS) -I$(OPENMPINCLUDE) -fopenmp -c -o $@ $<

darts:
	$(OMP2CD) $(OMP2CDFLAGS) $(SOURCES) -- $(CLANGFLAGS) $(CFLAGS)
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

clean-darts:
	rm -f ./*.output.darts.*

clean:
	rm -f *~ *.o $(MTARGET)
