import pandas as pd
import matplotlib.pyplot as plt

o = pd.read_csv("openmp_mm.csv")
m = pd.read_csv("mpi_mm.csv")

plt.figure()
plt.plot(o['threads'], o['time'], marker='o')
plt.xlabel("Number of Threads")
plt.ylabel("Time (s)")
plt.title("OpenMP Matrix Multiplication")
plt.grid()
plt.savefig("openmp_mm.png")


plt.figure()
plt.plot(m['procs'], m['time'], marker='o')
plt.xlabel("Number of Processes")
plt.ylabel("Time (s)")
plt.title("MPI Matrix Multiplication")
plt.grid()
plt.savefig("mpi_mm.png")

print("Graphs saved!")
