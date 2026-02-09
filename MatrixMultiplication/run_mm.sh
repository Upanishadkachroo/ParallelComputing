#!/bin/bash

N=600
A="mat${N}_A.csv"
B="mat${N}_B.csv"


#measures threads vs time 
echo "threads,time" > openmp_mm.csv

for t in 1 2 4 8 12 16
do
    time=$(./mm_openmp $A $B $N $t)
    echo "$t,$time" >> openmp_mm.csv
done


echo "procs,time" > mpi_mm.csv

for p in 1 2 4 8 12 16
do
    time=$(mpirun -np $p ./mm_mpi $A $B $N)
    echo "$p,$time" >> mpi_mm.csv
done
