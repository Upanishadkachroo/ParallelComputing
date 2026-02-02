#!/bin/bash
DATA=$1 #dataset.csv
K=$2 
IT=$3
PROCS=$4 #here number of prcoessor

echo "procs,time_seconds" > mpi_results.csv

for p in $(echo $PROCS | tr "," " ")
do
  mpirun -np $p ./kmeans_mpi $DATA $K $IT | \ awk '/TIME_SECONDS/ {print "'$p'," $2}' >> mpi_results.csv
done

#mpirun -np 4 ./kmeans_mpi dataset.csv 6 50
