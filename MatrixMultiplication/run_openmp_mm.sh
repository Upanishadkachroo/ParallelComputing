#!/bin/bash
N=$1
IT=$2
THREADS=$3

echo "threads,time_seconds" > openmp_mm.csv

for t in $(echo $THREADS | tr "," " ")
do
   ./matmul_openmp $N $t $IT | \ awk '/TIME_SECONDS/ {print "'$t'," $2}' >> openmp_mm.csv
done
