include ../../Makefile.def

SHELL=/bin/bash

TARGET=comd.exe
SOURCES=cmdLineParser.c CoMD.c decomposition.c eam.c haloExchange.c initAtoms.c linkCells.c ljForce.c mycommand.c parallel.c performanceTimers.c random.c timestep.c yamlOutput.c

.PHONY: all darts omp clean-darts clean

all: omp

darts:
	$(OMP2CD) $(OMP2CDFLAGS) $(SOURCES) -- $(CLANGFLAGS)
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

omp: $(SOURCES:.c=.o)
	$(CC) -O3 $(SOURCES:.c=.o) $(LDFLAGS_OPENMP) -o $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS_OPENMP) -o $@
	
clean-darts:
	rm -f ./*.output.darts.*

clean:
	rm -f *~ *.o $(TARGET) *.yaml
