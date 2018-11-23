#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


N="10000 1000000 100000000" 
THREADS="1 2 4 8 12 16"


for n in $N;
do
    for t in $THREADS;
    do
	./reduce $n $t  >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}
    done
done
	     
