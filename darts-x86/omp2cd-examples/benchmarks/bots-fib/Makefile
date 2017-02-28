##############################################################################################
#  This program is part of the Barcelona OpenMP Tasks Suite                                  #
#  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  #
#  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   #
#                                                                                            #
#  This program is free software; you can redistribute it and/or modify                      #
#  it under the terms of the GNU General Public License as published by                      #
#  the Free Software Foundation; either version 2 of the License, or                         #
#  (at your option) any later version.                                                       #
#                                                                                            #
#  This program is distributed in the hope that it will be useful,                           #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of                            #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                             #
#  GNU General Public License for more details.                                              #
#                                                                                            #
#  You should have received a copy of the GNU General Public License                         #
#  along with this program; if not, write to the Free Software                               #
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            #
##############################################################################################

include ../../Makefile.def

SOURCES=bots_main.c bots_common.c fib.c
TARGET=bots-fib.exe

all: omp

darts:
	$(OMP2CD) $(OMP2CDFLAGS) $(SOURCES) -d debug.txt -- $(CLANGFLAGS)
	@clang-format -i -style="{BasedOnStyle: WebKit, ColumnLimit: 100}" ./*.output.darts.*

omp: $(SOURCES:.c=.o)
	$(CC) -O3 $(SOURCES:.c=.o) $(LDFLAGS_OPENMP) -o $(TARGET)

%.o: %.c
	$(CC) -c $< $(CFLAGS_OPENMP) -o $@
	
clean-darts:
	rm -f ./*.output.darts.*

clean:
	rm -f *~ *.o $(TARGET)




