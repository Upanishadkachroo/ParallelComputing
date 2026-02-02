import matplotlib.pyplot as plt
import csv

def plot(file,x,y,title,out):
    xs,ys=[],[]

    with open(file) as f:
        r=csv.DictReader(f)
        for row in r:
            xs.append(float(row[x]))
            ys.append(float(row[y]))

    plt.figure()
    plt.plot(xs,ys,marker='o')
    plt.xlabel(x)
    plt.ylabel("Time (seconds)")
    plt.title(title)
    plt.grid(True)
    plt.savefig(out)
    print("Saved:",out)

plot("openmp_results.csv","threads","time_seconds",
     "OpenMP: Threads vs Time","openmp.png")

plot("mpi_results.csv","procs","time_seconds",
     "MPI: Processes vs Time","mpi.png")
