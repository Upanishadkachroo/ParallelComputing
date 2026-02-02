#!/bin/bash
N=$1
IT=$2
PROCS=$3

echo "procs,time_seconds" > mpi_mm.csv

for p in $(echo $PROCS | tr "," " ")
do
   mpirun -np $p ./matmul_mpi $N $IT | \  awk '/TIME_SECONDS/ {print "'$p'," $2}' >> mpi_mm.csv
done
