if [ ! -e .passed_prefixsum ] ;
then
    echo "Must pass \`make test_prefixsum\` before queueing jobs"
    exit 1
fi
qsub -q mamba -d $(pwd) -l walltime=1:00:00,nodes=1:ppn=16 util/bench_prefixsum.sh
echo "-----------------------"
echo
echo "Once the jobe COMPLETES, plot with \`make plot_prefixsum\`"
