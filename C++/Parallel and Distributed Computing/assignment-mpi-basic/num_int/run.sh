#!/bin/sh

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

P=${PBS_NP}

mpirun ./mpi_num_int 1 0 10 100000000 1000 2> ${RESULTDIR}/${P}
