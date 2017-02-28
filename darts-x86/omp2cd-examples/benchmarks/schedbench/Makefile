include ../../Makefile.def

SHELL=/bin/bash

TARGET=schedbench.exe
SOURCES=schedbench.c common.c

.PHONY: all darts omp clean-darts clean

all: omp

darts:
	$(OMP2CD) $(OMP2CDFLAGS) $(SOURCES) -- $(CLANGFLAGS) -DOMPVER2 
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

omp: $(SOURCES:.c=.o)
	$(CC) -O3 $(SOURCES:.c=.o) $(LDFLAGS_OPENMP) -o $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS_OPENMP) -o $@ -DOMPVER2 

clean-darts:
	rm -f ./*.output.darts.*
	
clean:
	rm -f *~ *.o $(TARGET) 
  
