for i in 1 2 4 6 8
do
  echo "Running with $i processes"
  mpirun -np $i ./your_program
done
