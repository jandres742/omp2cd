include ../../Makefile.def

SHELL=/bin/bash

TARGET=arraybench.exe
SOURCES=arraybench.c common.c

IDA=1024

#inline-omp does not work properly with array variables in copyprivate clauses, which is why
#array_bench_1 does not use that option

.PHONY: all darts omp clean-darts clean

all: omp

darts:
	$(OMP2CD) -balanceOMPFor $(SOURCES) -- $(CLANGFLAGS) -DIDA=$(IDA) -DOMPVER2 
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

omp: $(SOURCES:.c=.o)
	$(CC) -O3 $(SOURCES:.c=.o) $(LDFLAGS_OPENMP) -o $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS_OPENMP) -o $@ -DIDA=$(IDA) -DOMPVER2 

clean-darts:
	rm -f ./*.output.darts.*

clean:
	rm -f *~ *.o $(TARGET) 
  
