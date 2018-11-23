CXX=mpicxx
CC=mpicc
LD=mpicxx

hello:

run_1x16: hello
	qsub -q mamba -l walltime=00:05:00 -d $(shell pwd) -l nodes=1:ppn=16 ./run.sh

run_2x8: hello
	qsub -q mamba -l walltime=00:05:00 -d $(shell pwd) -l nodes=2:ppn=8 ./run.sh

run_3x12: hello
	qsub -q mamba -l walltime=00:05:00 -d $(shell pwd) -l nodes=3:ppn=12 ./run.sh
