include ../../Makefile.def

CC=clang

SHELL=/bin/bash

TARGET=ompcodeletclause.exe
SOURCES=ompcodeletclause.c

CLANGFLAGS += -DUSECODELETS
CFLAGS_OPENMP += -DUSECODELETS

.PHONY: all darts omp clean-darts clean

all: omp

darts:
	$(OMP2CD) -balanceOMPFor -IG=0 -inlineOMP $(SOURCES) -- $(CLANGFLAGS)
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

omp: $(SOURCES:.c=.o)
	$(CC) -O3 $(SOURCES:.c=.o) $(LDFLAGS_OPENMP) -o $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS_OPENMP) -o $@

clean-darts:
	rm -f ./*.output.darts.*
	
clean:
	rm -f *~ *.o $(TARGET)
