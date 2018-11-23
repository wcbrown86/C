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


N="10000 1000000 100000000" # 1000000000"
THREADS="1 2 4 8 12 16"


for n in $N;
do
    for t in $THREADS;
    do
	./reduce $n $t static -1 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_static_-1
#        if $n -ne 1000000000 ;
#        then
	./reduce $n $t dynamic 1 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_dynamic_1
#        fi
	./reduce $n $t dynamic 1000 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_dynamic_1000
	./reduce $n $t dynamic 100000 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_dynamic_100000	
    done
done
	     
