import matplotlib.pyplot as plt
import csv

def plot(file,x,title,out):
    xs,ys=[],[]

    with open(file) as f:
        r=csv.DictReader(f)
        for row in r:
            xs.append(float(row[x]))
            ys.append(float(row["time_seconds"]))

    plt.figure()
    plt.plot(xs,ys,marker='o')
    plt.xlabel(x)
    plt.ylabel("Time (seconds)")
    plt.title(title)
    plt.grid(True)
    plt.savefig(out)
    print("Saved:",out)

plot("openmp_mm.csv","threads",
     "OpenMP Matrix Mul","openmp_mm.png")

plot("mpi_mm.csv","procs",
     "MPI Matrix Mul","mpi_mm.png")
