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


NS="10000 1000000"
THREADS="1 2 4 8 12 16"


# bench
for n in $NS;
do
    for t in $THREADS;
    do
	./bubblesort $n $t >/dev/null 2> ${RESULTDIR}/bubblesort_${n}_${t}
    done
done
	     
