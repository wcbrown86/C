CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=libintegrate.a libfunctions.a libgen.a 
LD=g++


UTILDIR=util


all: reduce mergesort bubblesort lcs approx


# archives

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o


# compile

reduce: reduce.o 
	$(LD) $(LDFLAGS) reduce.o $(ARCHIVES) -o reduce

reduce_seq: reduce_seq.o
	$(LD) $(LDFLAGS) reduce_seq.o $(ARCHIVES) -o reduce_seq

mergesort: mergesort.o
	$(LD) $(LDFLAGS) mergesort.o $(ARCHIVES) -o mergesort

mergesort_seq: mergesort_seq.o
	$(LD) $(LDFLAGS) mergesort_seq.o $(ARCHIVES) -o mergesort_seq

bubblesort: bubblesort.o
	$(LD) $(LDFLAGS) bubblesort.o $(ARCHIVES) -o bubblesort

bubblesort_seq: bubblesort_seq.o
	$(LD) $(LDFLAGS) bubblesort_seq.o $(ARCHIVES) -o bubblesort_seq

lcs: lcs.o
	$(LD) $(LDFLAGS) lcs.o $(ARCHIVES) -o lcs
	
lcs_seq: lcs_seq.o
	$(LD) $(LDFLAGS) lcs_seq.o $(ARCHIVES) -o lcs_seq


# test code
test_reduce: reduce approx
	./$(UTILDIR)/test_reduce.sh

test_mergesort: mergesort
	./$(UTILDIR)/test_mergesort.sh

test_bubblesort: bubblesort
	./$(UTILDIR)/test_bubblesort.sh

test_lcs: lcs
	./$(UTILDIR)/test_lcs.sh


# run benchmarks
bench_sequential: reduce_seq mergesort_seq bubblesort_seq lcs_seq
	./$(UTILDIR)/queue_sequential.sh

bench_reduce: reduce
	./$(UTILDIR)/queue_reduce.sh

bench_mergesort: mergesort
	./$(UTILDIR)/queue_mergesort.sh

bench_bubblesort: bubblesort
	./$(UTILDIR)/queue_bubblesort.sh

bench_lcs: lcs
	./$(UTILDIR)/queue_lcs.sh


# plot results
plot_reduce: 
	./$(UTILDIR)/plot_reduce.sh

plot_mergesort:  
	./$(UTILDIR)/plot_mergesort.sh

plot_bubblesort:  
	./$(UTILDIR)/plot_bubblesort.sh

plot_lcs:  
	./$(UTILDIR)/plot_lcs.sh


assignment-openmp-loop.tgz: Makefile approx.cpp \
                            libgen.a \
                            util \
                            reduce_seq.cpp reduce.cpp \
                            mergesort_seq.cpp mergesort.cpp \
                            bubblesort_seq.cpp bubblesort.cpp \
                            lcs_seq.cpp lcs.cpp \
                            assignment-openmp-loop.pdf
	tar zcvf assignment-openmp-loop.tgz \
                            Makefile approx.cpp \
                            libgen.a \
                            util \
                            reduce_seq.cpp reduce.cpp \
                            mergesort_seq.cpp mergesort.cpp \
                            bubblesort_seq.cpp bubblesort.cpp \
                            lcs_seq.cpp lcs.cpp \
                            assignment-openmp-advanced.pdf


clean:
	-rm *.o

distclean:
	-rm *.sh.*


.PHONY: reduce mergesort bubblesort lcs reduce_seq mergesort_seq bubblesort_seq lcs_seq 
