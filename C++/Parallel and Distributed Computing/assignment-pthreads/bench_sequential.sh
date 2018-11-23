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
    


INTENSITIES="1 10 100 1000"
THREADS="1 2 4 8 12 16"
NSPLOT="1 100 10000 1000000 100000000"

echo running on ${h}
echo starting bench at $(date)

for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	./sequential 1 0 10 ${n} ${intensity} 2>${RESULTDIR}/sequential_${n}_${intensity}  >/dev/null
    done
done

echo ending bench at $(date)
