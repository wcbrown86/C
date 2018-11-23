if [ ! -e .passed_reduce ] ;
then
    echo "Must pass \`make test_reduce\` before queueing jobs"
    exit 1
fi

qsub -q mamba -d $(pwd) -l walltime=1:00:00,nodes=1:ppn=16 util/bench_reduce.sh
echo "----------------------"
echo
echo "Once the job COMPLETES, plot with \`make plot_reduce\`"
