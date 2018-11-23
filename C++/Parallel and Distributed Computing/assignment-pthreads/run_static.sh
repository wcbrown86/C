#!/bin/sh


RESULTDIR=result/
h=`hostname`

echo running on ${h}

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -e .passed_dynamic ];
then
    echo "You must pass all test before submitting"
    mkdir ${RESULTDIR}
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi
    

INTENSITIES="1 10 100 1000"
#THREADS="1 2 4 8 12 16"
SYNCS="iteration thread"
NSPLOT="1 100 10000 1000000 100000000"



# ensure sequential data exists before running 
# assumes the $INTENSITIES and $NS are the same in bench_sequential.sh

echo before starting time is $(date)
echo THREAD=${THREAD}

for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	for sync in ${SYNCS};
	do
	
#		qsub -q mamba -d $(pwd) -l nodes=1:ppn=${thread} -v ITENSITY=${intensity},N=${n},THREAD=${thread},SYNC=${sync},GRAN=${gran} run_dynamic.sh
 	    ./static_sched 1 0 10 ${n} ${intensity} ${THREAD} ${sync} 2>${RESULTDIR}/static_${n}_${intensity}_${THREAD}_${sync}  >/dev/null 

        done
    done
done

echo after ending time is $(date)
