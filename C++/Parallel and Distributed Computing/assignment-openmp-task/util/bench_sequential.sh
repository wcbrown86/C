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

# LCS N's
LCS_NS="1000 100000" #1000000"

# sorts N's
MS_NS="10000 1000000 100000000 1000000000"
BS_NS="10000 1000000"


# reduce sequential
for n in $R_NS ;
do
    ./reduce_seq ${n}   >/dev/null 2> ${RESULTDIR}/reduction_seq_${n}
done

# longest common subsequence sequential
for m in $LCS_NS;
do
    for n in $LCS_NS;
    do 
        ./lcs_seq ${m} ${n} >/dev/null 2> ${RESULTDIR}/lcs_seq_${m}_${n}
    done
done
#./lcs_seq 1000 1000 >/dev/null 2> ${RESULTDIR}/lcs_seq_1000_1000
#./lcs_seq 1000 100000 >/dev/null 2> ${RESULTDIR}/lcs_seq_1000_100000
#./lcs_seq 100000 100000 >/dev/null 2> ${RESULTDIR}/lcs_seq_100000_100000


# mergesort sequential
# bubblesort sequential
for n in $MS_NS ; 
do

  ./mergesort_seq ${n}  >/dev/null 2> ${RESULTDIR}mergesort_seq_${n}
done

for n in $BS_NS ; 
do
  ./bubblesort_seq ${n}  >/dev/null 2> ${RESULTDIR}/bubblesort_seq_${n}
done

