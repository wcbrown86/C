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


# reduce N's 
R_NS="10000 1000000 100000000"
# numint N's
NI_NS="1000 100000 100000000"
ITENSITIES="10 1000"
# prefixsum N's
PS_NS="10000 1000000 100000000 1000000000"
# mergesort N's
MS_NS="10000 1000000 100000000 1000000000"



# reduce sequential
for n in $R_NS ;
do
    ./reduce_seq ${n}   >/dev/null 2> ${RESULTDIR}/reduction_seq_${n}
done


# numerical integration sequential
for n in $NI_NS;
do
    for iten in ${ITENSITIES};
    do 
        ./numint_seq 1 1 10 ${n} ${iten} >/dev/null 2> ${RESULTDIR}/numint_seq_${n}_${iten}
    done
done

# prefixsum sequential
for n in $PS_NS ;
do 
    ./prefixsum_seq ${n} >/dev/null 2> ${RESULTDIR}/prefixsum_seq_${n}
done

# mergesort sequential
for n in $MS_NS ; 
do
    ./mergesort_seq ${n}  >/dev/null 2> ${RESULTDIR}/mergesort_seq_${n}
done


