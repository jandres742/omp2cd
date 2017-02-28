SUBDIRS = copyprivate-test \
critical-atomic-test \
fibtasks-test \
lastprivate-test \
masterthread-test \
matmul-test \
ompcodeletclause-test \
parallelfor-test \
reduce-omp-test \
task-depend-test \
tasks-test \
threadprivate-test \
tree-traversal-tasks-test

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
