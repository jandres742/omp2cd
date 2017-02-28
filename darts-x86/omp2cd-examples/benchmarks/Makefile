SUBDIRS = array_bench \
bots-alignment-single \
bots-fib \
bots-fft \
bots-nqueens \
bots-sparselu-single \
bots-sort \
bots-strassen \
bots-uts \
clomp_v1.2 \
CoMD-master-openmp \
CoSP2-master-openmp \
DC \
graph500-2.1.4 \
IS \
scf-openmp \
schedbench \
syncbench \
XSBench

all:
	@for dir in $(SUBDIRS); do \
		cd $$dir; make; cd ../;\
	done

darts:
	@for dir in $(SUBDIRS); do \
		cd $$dir; make darts; cd ../;\
	done

clean-darts:
	@for dir in $(SUBDIRS); do \
		cd $$dir; make clean-darts; cd ../;\
	done

clean:
	@for dir in $(SUBDIRS); do \
		cd $$dir; make clean; cd ../;\
	done