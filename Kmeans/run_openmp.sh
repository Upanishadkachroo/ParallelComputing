#!/bin/bash
DATA=$1 #dataset.csv
K=$2 #no. of clusters
IT=$3 #max. no. of iterations
THREADS=$4 #number of threads 

echo "threads,time_seconds" > openmp_results.csv //separate file to store results

for t in $(echo $THREADS | tr "," " ")
do
  ./kmeans_openmp $DATA $K $IT $t | \ awk '/TIME_SECONDS/ {print "'$t'," $2}' >> openmp_results.csv
done


# inside cpp
# argv[1] = dataset.csv
# argv[2] = 6
# argv[3] = 50
# argv[4] = 4   → number of threads
# omp_set_num_threads(4);
