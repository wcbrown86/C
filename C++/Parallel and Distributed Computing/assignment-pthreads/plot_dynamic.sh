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


INTENSITIES="1 10 100 1000"
THREADS="1 2 4 8 12 16"
GRANS="1 100 10000 1000000"
SYNCS="thread chunk"
NSPLOT=" 1 100 10000 1000000 100000000"




for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
        if [[ ! -s "${RESULTDIR}sequential_${n}_${intensity}" ]];
        then
           echo Run 'make seq' before plotting 
           exit
        fi
    done
done

#preparing files for plotting

for intensity in $INTENSITIES;
do
    for thread in ${THREADS};
    do
	for n in $NSPLOT;
	do
	    for sync in ${SYNCS};
	    do
		for gran in ${GRANS};
		do
		    #output in format "gran seqtime partime"
		    echo ${gran} \
			 $(cat ${RESULTDIR}/sequential_${n}_${intensity}) \
			 $(cat ${RESULTDIR}/dynamic_${n}_${intensity}_${thread}_${sync}_${gran})
		done > ${RESULTDIR}/speedupc_dynamic_${n}_${thread}_${intensity}_${sync}
	    done
	done
    done
done


for intensity in $INTENSITIES;
do
    for gran in ${GRANS};
    do
	for n in $NSPLOT;
	do
	    for sync in ${SYNCS};
	    do
		for thread in ${THREADS};
		do
		    #output in format "gran seqtime partime"
		    echo ${thread} \
			 $(cat ${RESULTDIR}/sequential_${n}_${intensity}) \
			 $(cat ${RESULTDIR}/dynamic_${n}_${intensity}_${thread}_${sync}_${gran})
		done > ${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_${sync}_${gran}
	    done
	done
    done
done
# file prepared


#Speedup plots

for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	for thread in ${THREADS};
	do
	    # GCMDSP="${GCMDSP} ; set key top left; \
            #                   set xlabel 'granularity'; \
            #                   set ylabel 'speedup'; \
            #                   set xrange [*:*]; \
            #                   set yrange [*:20]; \
#                   set title'n=$n intensity=$intensity thread=${thread}'; \
#         plot '${RESULTDIR}/speedupc_dynamic_${n}_${thread}_${intensity}_iteration' u 1:(\$2/\$3) t 'iteration' lc 1, \
            #              '${RESULTDIR}/speedupc_dynamic_${n}_${thread}_${intensity}_chunk' u 1:(\$2/\$3) t 'chunk' lc 4, \
            #              '${RESULTDIR}/speedupc_dynamic_${n}_${thread}_${intensity}_thread' u 1:(\$2/\$3) lc 3 t 'thread'; "

	    GCMDSP="${GCMDSP} ; set key top left; \
                              set xlabel 'granularity'; \
                              set ylabel 'speedup'; \
                              set xrange [*:*]; \
                              set yrange [*:20]; \
                              set title'n=$n intensity=$intensity thread=${thread}'; \
                    plot '${RESULTDIR}/speedupc_dynamic_${n}_${thread}_${intensity}_chunk' u 1:(\$2/\$3) lc 4 lw 3 t 'chunk' , \
                         '${RESULTDIR}/speedupc_dynamic_${n}_${thread}_${intensity}_thread' u 1:(\$2/\$3) lc 3 lw 3 t 'thread'; "
	done
    done
done

for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	    # GCMDSPN="${GCMDSPN} ; set key top left; \
            #                       set xlabel 'threads'; \
            #                       set ylabel 'speedup'; \
            #                       set xrange [*:*]; \
            #                       set yrange [*:20]; \
            #                       set title 'thread=${thread} intensity=${intensity} granularity=${gran}'; \
            #     plot '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_iteration_${gran}' u 1:(\$2/\$3) t 'iteration' lc 1, \
            #          '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_chunk_${gran}' u 1:(\$2/\$3) t 'chunk' lc 4, \
            #          '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_thread_${gran}' u 1:(\$2/\$3) lc 3 t 'thread'; "

	    GCMDSPN="${GCMDSPN} ; set key top left; \
				  set key title 'granularity'; \
                                  set xlabel 'threads'; \
                                  set ylabel 'speedup'; \
                                  set xrange [*:*]; \
                                  set yrange [*:20]; \
                                  set title 'chunk (n=${n} intensity=${intensity})';"
	    GCMDSPN="${GCMDSPN} ; plot "

 	c=1
	for gran in ${GRANS};
	do
            GCMDSPN="${GCMDSPN} '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_chunk_${gran}' u 1:(\$2/\$3) t '${gran}' lc ${c} lw 3," 
	    c=`expr $c + 1`
#                     '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_thread_${gran}' u 1:(\$2/\$3) lc 3 t 'thread'; "
	done
    done
done

#speedup as a function of n


for intensity in $INTENSITIES;
do
    for n in $NSPLOT;
    do
	    # GCMDSPN="${GCMDSPN} ; set key top left; \
            #                       set xlabel 'threads'; \
            #                       set ylabel 'speedup'; \
            #                       set xrange [*:*]; \
            #                       set yrange [*:20]; \
            #                       set title 'thread=${thread} intensity=${intensity} granularity=${gran}'; \
            #     plot '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_iteration_${gran}' u 1:(\$2/\$3) t 'iteration' lc 1, \
            #          '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_chunk_${gran}' u 1:(\$2/\$3) t 'chunk' lc 4, \
            #          '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_thread_${gran}' u 1:(\$2/\$3) lc 3 t 'thread'; "

	    GCMDSPPN="${GCMDSPPN} ; set key top left; \
				  set key title 'granularity'; \
                                  set xlabel 'threads'; \
                                  set ylabel 'speedup'; \
                                  set xrange [*:*]; \
                                  set yrange [*:20]; \
                                  set title 'thread (n=${n} intensity=${intensity})'; "

	    GCMDSPPN="${GCMDSPPN} ; plot "
 	c=1
	for gran in ${GRANS};
	do

            GCMDSPPN="${GCMDSPPN} '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_thread_${gran}' u 1:(\$2/\$3) t '${gran}' lc ${c} lw 3," 
	    c=`expr $c + 1`
#                     '${RESULTDIR}/speedupt_dynamic_${n}_${intensity}_thread_${gran}' u 1:(\$2/\$3) lc 3 t 'thread'; "
	done
    done
done


# create plots

gnuplot <<EOF
set terminal pdf

set output '${PLOTDIR}/dynamic_chunk.pdf'

set style data linespoints

${GCMDSPN}

EOF

gnuplot <<EOF
set terminal pdf

set output '${PLOTDIR}/dynamic_thread.pdf'

set style data linespoints

${GCMDSPPN}

EOF

gnuplot <<EOF
set terminal pdf

set output '${PLOTDIR}/dynamic_compare.pdf'

set style data linespoints

${GCMDSP}

EOF
