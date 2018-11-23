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

NS="1000 100000 100000000"
THREADS="1 2 4 8 12 16"
GRANS="1 1000"
INTENSITIES="10 1000"


	     
# format output
for n in $NS;
do
    for inten in ${INTENSITIES}
    do
	for gran in ${GRANS}
	do
	    for t in ${THREADS};
	    do
		#output in format "thread seq par"
		echo ${t} \
		     $(cat ${RESULTDIR}/numint_seq_${n}_${inten}) \
		     $(cat ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten})
	    done   > ${RESULTDIR}/speedup_numint_${n}_${gran}_${inten}
	done
    done
done

for t in ${THREADS};
do 
    for inten in ${INTENSITIES}
    do
        for gran in ${GRANS};
        do
            for n  in ${NS};
            do
                echo ${n} \
		     $(cat ${RESULTDIR}/numint_seq_${n}_${inten}) \
		     $(cat ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten})
            done >   ${RESULTDIR}/speedup_numint_thread_${t}_${gran}_${inten}
        done
    done
done




# plot
for n in $NS ; 
do 
    GSP="${GSP} ; set title 'n=$n' ; plot "
    c=1
    for gran in ${GRANS} ; 
    do 
        for inten in ${INTENSITIES} ;
        do

            GSP="${GSP} '${RESULTDIR}/speedup_numint_${n}_${gran}_${inten}' u 1:(\$2/\$3) t 'gran=${gran} inten=${inten}' lc ${c} lw 3,  "
            c=`expr $c + 1`
        done
    done
done



for t in ${THREADS};
do 
    for inten in ${INTENSITIES}
    do
        for gran in ${GRANS};
        do
            GTSP="${GTSP} ; set title 'threads=${t}, gran=${gran}, inten=${inten}' ; \
                            set key top left; \
                            set xlabel 'N'; \
                            set ylabel 'speedup'; \
                            set xrange [*:1000000000]; \
                            set logscale x 10; \
                            set yrange [0:20]; \
                            set ytics 2; "
            GTSP="${GTSP} plot '${RESULTDIR}/speedup_numint_thread_${t}_${gran}_${inten}' u 1:(\$2/\$3) t 'thread' lc 1 lw 3;"

        done
    done
done


#set title 'n=1000' ;
#plot '${RESULTDIR}/speedup_numint_1000_1_10' u 1:(\$2/\$3) t 'gran=1 inten=10' lc 1 lw 3, \
#     '${RESULTDIR}/speedup_numint_1000_1_1000' u 1:(\$2/\$3) t 'gran=1 inten=1000' lc 3 lw 3, \
#     '${RESULTDIR}/speedup_numint_1000_1000_10' u 1:(\$2/\$3) t 'gran=1000 inten=10' lc 4 lw 3, \
#     '${RESULTDIR}/speedup_numint_1000_1000_1000' u 1:(\$2/\$3) t 'gran=1000 inten=1000' lc 5 lw 3
#
#set title 'n=100000000' ;
#plot '${RESULTDIR}/speedup_numint_100000000_1_10' u 1:(\$2/\$3) t 'gran=1 inten=10' lc 1 lw 3, \
#     '${RESULTDIR}/speedup_numint_100000000_1_1000' u 1:(\$2/\$3) t 'gran=1 inten=1000' lc 3 lw 3, \
#     '${RESULTDIR}/speedup_numint_100000000_1000_10' u 1:(\$2/\$3) t 'gran=1000 inten=10' lc 4 lw 3, \
#     '${RESULTDIR}/speedup_numint_100000000_1000_1000' u 1:(\$2/\$3) t 'gran=1000 inten=1000' lc 5 lw 3


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}numint_plots.pdf'
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
set output '${PLOTDIR}numint_plots_thread.pdf'
set style data linespoints

${GTSP}

EOF

