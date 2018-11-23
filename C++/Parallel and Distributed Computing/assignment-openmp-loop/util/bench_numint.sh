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


NS="1000 100000 100000000"
THREADS="1 2 4 8 12 16"
GRANS="1 1000"
INTENSITIES="10 1000"


# bench
for n in $NS;
do
    for inten in ${INTENSITIES}
    do
	./numint_seq 1 1 10 ${n} ${inten}  >/dev/null 2> ${RESULTDIR}/numint_${n}_${inten}
	
	for gran in ${GRANS}
	do
	    for t in ${THREADS};
	    do
		./numint 1 1 10 ${n} ${inten} ${t} dynamic ${gran} >/dev/null 2> ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten}
	    done
	done
    done
done
	     
