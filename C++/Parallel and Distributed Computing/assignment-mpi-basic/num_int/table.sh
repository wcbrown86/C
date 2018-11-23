#!/bin/sh

RESULTDIR=result/

for i in 1 2 4 8 16 32 36;
do
    echo $i $(cat ${RESULTDIR}/${i})
done

