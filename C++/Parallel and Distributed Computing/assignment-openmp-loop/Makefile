CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=libintegrate.a libfunctions.a libgen.a 
LD=g++


UTILDIR=util


all: reduce mergesort prefixsum numint numint_seq prefixsum_seq approx


libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

libintegrate.a: sequential_lib.o
	ar rcs libintegrate.a sequential_lib.o

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o


reduce: reduce.o
	$(LD) $(LDFLAGS) reduce.o $(ARCHIVES) -o reduce

reduce_seq: reduce_seq.o
	$(LD) $(LDFLAGS) reduce_seq.o $(ARCHIVES) -o reduce_seq

mergesort: mergesort.o
	$(LD) $(LDFLAGS) mergesort.o $(ARCHIVES) -o mergesort

mergesort_seq: mergesort_seq.o
	$(LD) $(LDFLAGS) mergesort_seq.o $(ARCHIVES) -o mergesort_seq

prefixsum: prefixsum.o
	$(LD) $(LDFLAGS) prefixsum.o $(ARCHIVES) -o prefixsum

prefixsum_seq: prefixsum_seq.o
	$(LD) $(LDFLAGS) prefixsum_seq.o $(ARCHIVES) -o prefixsum_seq

numint: numint.o
	$(LD) $(LDFLAGS) numint.o $(ARCHIVES) -o numint

numint_seq: numint_seq.o
	$(LD) $(LDFLAGS) numint_seq.o $(ARCHIVES) -o numint_seq


# test code
test_reduce: reduce approx
	./$(UTILDIR)/test_reduce.sh

test_numint: numint approx
	./$(UTILDIR)/test_numint.sh

test_prefixsum: prefixsum
	./$(UTILDIR)/test_prefixsum.sh

test_mergesort: mergesort
	./$(UTILDIR)/test_mergesort.sh


# run benchmarks
bench_sequential: reduce_seq numint_seq prefixsum_seq mergesort_seq
	./$(UTILDIR)/queue_sequential.sh

bench_reduce: reduce
	./$(UTILDIR)/queue_reduce.sh

bench_numint: numint
	./$(UTILDIR)/queue_numint.sh

bench_prefixsum: prefixsum
	./$(UTILDIR)/queue_prefixsum.sh

bench_mergesort: mergesort
	./$(UTILDIR)/queue_mergesort.sh


# plot results
plot_reduce: 
	./$(UTILDIR)/plot_reduce.sh

plot_numint:  
	./$(UTILDIR)/plot_numint.sh

plot_prefixsum: 
	./$(UTILDIR)/plot_prefixsum.sh

plot_mergesort:  
	./$(UTILDIR)/plot_mergesort.sh

assignment-openmp-loop.tgz: Makefile approx.cpp \
                            libfunctions.a libgen.a libintegrate.a \
                            util \
                            reduce_seq.cpp reduce.cpp \
                            numint_seq.cpp numint.cpp \
                            prefixsum_seq.cpp prefixsum.cpp \
                            mergesort_seq.cpp mergesort.cpp \
                            assignment-openmp-loop.pdf
	tar zcvf assignment-openmp-loop.tgz \
                            Makefile approx.cpp \
                            libfunctions.a libgen.a libintegrate.a \
                            util \
                            reduce_seq.cpp reduce.cpp \
                            numint_seq.cpp numint.cpp \
                            prefixsum_seq.cpp prefixsum.cpp \
                            mergesort_seq.cpp mergesort.cpp \
                            assignment-openmp-loop.pdf


clean:
	-rm *.o

distclean:
	-rm *.sh.*


.PHONY: reduce mergesort prefixsum numint
