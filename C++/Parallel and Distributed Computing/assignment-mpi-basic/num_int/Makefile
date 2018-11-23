CXX=mpicxx
CC=mpicc
LD=mpicxx
CXXFLAGS=-std=c++14
LDLIBS=libfunctions.a

mpi_num_int:

run: run_1x1 run_1x2 run_1x4 run_1x8 run_1x16 run_2x16 run_3x12
	echo

run_1x1: mpi_num_int
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=1 ./run.sh

run_1x2: mpi_num_int
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=2 ./run.sh

run_1x4: mpi_num_int
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=4 ./run.sh

run_1x8: mpi_num_int
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=8 ./run.sh

run_1x16: mpi_num_int
	qsub -q mamba -l walltime=00:10:00 -d $(shell pwd) -l nodes=1:ppn=16 ./run.sh

run_2x16: mpi_num_int
	qsub -q mamba -l walltime=00:10:00 -d $(shell pwd) -l nodes=2:ppn=16 ./run.sh

run_3x12: mpi_num_int
	qsub -q mamba -l walltime=00:10:00 -d $(shell pwd) -l nodes=3:ppn=12 ./run.sh

table:
	./table.sh
