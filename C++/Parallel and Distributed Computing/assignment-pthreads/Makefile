CFLAGS=-O3 -std=c11 -fPIC -pthread
CXXFLAGS=-O3 -std=c++11 -g -fPIC -pthread
LDFLAGS=-pthread
LDLIBS=libintegrate.a libfunctions.a
LD=g++

all: static_sched

static_sched: static_sched.o
	$(LD) $(LDFLAGS) static_sched.o $(LDLIBS) -o static_sched

dynamic_sched: dynamic_sched.o
	$(LD) $(LDFLAGS) dynamic_sched.o $(LDLIBS) -o dynamic_sched

libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

libintegrate.a: sequential_lib.o
	ar rcs libintegrate.a sequential_lib.o

sequential: sequential.o 
	$(LD) $(LDFLAGS) sequential.o $(LDLIBS) -o sequential

bench_dynamic: dynamic_sched
	./queue_dynamic.sh 

bench_static: static_sched
	./queue_static.sh 

bench_sequential: sequential
	./queue_sequential.sh 

test_static: static_sched approx
	./test_static.sh

test_dynamic: dynamic_sched approx
	./test_dynamic.sh

plot_static: 
	./plot_static.sh

plot_dynamic: 
	./plot_dynamic.sh

clean:
	./clean.sh

assignment-pthreads.pdf: assignment-pthreads.tex
	pdflatex assignment-pthreads.tex

assignment-pthreads.tgz: assignment-pthreads.pdf \
                         Makefile approx.cpp clean.sh libfunctions.a libintegrate.a \
                         sequential.cpp queue_sequential.sh bench_sequential.sh \
                         static_sched.cpp test_static_cases.txt test_static.sh run_static.sh queue_static.sh plot_static.sh \
                         dynamic_sched.cpp test_dynamic_cases.txt test_dynamic.sh run_dynamic.sh queue_dynamic.sh plot_dynamic.sh
	tar zcvf assignment-pthreads.tgz \
                         assignment-pthreads.pdf \
                         Makefile approx.cpp clean.sh libfunctions.a libintegrate.a \
                         sequential.cpp queue_sequential.sh bench_sequential.sh \
                         static_sched.cpp test_static_cases.txt test_static.sh run_static.sh queue_static.sh plot_static.sh \
                         dynamic_sched.cpp test_dynamic_cases.txt test_dynamic.sh run_dynamic.sh queue_dynamic.sh plot_dynamic.sh
