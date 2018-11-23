qsub -q mamba -d $(pwd) -l nodes=1:ppn=16 util/bench_sequential.sh
