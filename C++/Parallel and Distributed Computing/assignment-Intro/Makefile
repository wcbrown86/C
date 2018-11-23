CFLAGS=-O3 -std=c11
CXXFLAGS=-O3 -std=c++11
LDFLAGS=
ARCHIVES=libfunctions.a
LD=g++

main: main.o
	$(LD) $(LDFLAGS) main.o $(ARCHIVES) -o main

prelim: prelim.o
	$(LD) $(LDFLAGS) prelim.o -o prelim

libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

test: main approx 
	./test.sh

bench: main 
	./bench.sh	

assignment-numericalintegration.pdf: assignment-numericalintegration.tex
	pdflatex assignment-numericalintegration.tex

assignment.tgz: assignment-numericalintegration.pdf \
	prelim.cpp queue_prelim.sh run_prelim.sh \
	main.cpp libfunctions.a Makefile \
	approx.cpp test.sh test_cases.txt \
	queue_job.pbs bench.sh run_bench.sh plot.sh 
	tar zcvf assignment.tgz \
	assignment-numericalintegration.pdf \
	prelim.cpp queue_prelim.sh run_prelim.sh \
	main.cpp libfunctions.a Makefile \
	approx.cpp test.sh test_cases.txt \
	queue_job.pbs bench.sh run_bench.sh plot.sh 
