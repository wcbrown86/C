#!/bin/sh

RESULTDIR=result/
PLOTDIR=plot/
h=`hostname`


if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

if [ ! -d ${PLOTDIR} ];
then
    mkdir ${PLOTDIR}
fi

NS="10000 1000000 100000000 1000000000"
THREADS="1 2 4 8 12 16"

	     
# format output
for n in $NS;
do
    for t in $THREADS;
    do
	#output in format "thread seq par"
	echo ${t} \
	     $(cat ${RESULTDIR}/prefixsum_seq_${n}) \
	     $(cat ${RESULTDIR}/prefixsum_${n}_${t})
    done   > ${RESULTDIR}/speedup_prefixsum_${n}
done

GSP="${GSP} ; set title 'prefixsum'; plot "
c=1
for n in $NS;
do
    GSP="${GSP} '${RESULTDIR}/speedup_prefixsum_${n}' u 1:(\$2/\$3) t 'n=$n' lc ${c} lw 3 , "
    c=`expr $c + 1`
done


for t in $THREADS;
do
    for n in $NS;
    do
       echo ${n} \
            $(cat ${RESULTDIR}/prefixsum_seq_${n}) \
            $(cat ${RESULTDIR}/prefixsum_${n}_${t})
    done > ${RESULTDIR}/speedup_prefixsum_thread_${t}
done

for t in $THREADS;
do 
    GTSP="${GTSP} ; set title 'prefixsum- ${t} threads'; \
                    set key top left; \
                    set xlabel 'N'; \
                    set ylabel 'speedup'; \
                    set xrange [*:*]; \
                    set logscale x 10; \
                    set yrange [0:20]; \
                    set ytics 2;"
    GTSP="${GTSP} plot '${RESULTDIR}/speedup_prefixsum_thread_${t}' u 1:(\$2/\$3) t 'threads=${t}' lc 1 lw 3 ;"
done


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}prefixsum_speedup_n.pdf'
set style data linespoints
set key top left;
set xlabel 'threads'; 
set ylabel 'speedup';
set xrange [0:20];
set yrange [0:20];
set ytics 2;

${GSP}

EOF

gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}prefixsum_speedup_thread.pdf'
set style data linespoints

${GTSP}
EOF
