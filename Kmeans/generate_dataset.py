import random
import csv

K = 6
N = 50000

centers=[(10,10),(40,40),(-20,30),(30,-10),(-30,-20),(0,0)]

with open("dataset.csv","w",newline='') as f:
    w=csv.writer(f)
    w.writerow(["x","y"])

    for i in range(N):
        cx,cy = centers[i%K]
        x = random.gauss(cx,2.5)
        y = random.gauss(cy,2.5)
        w.writerow([x,y])

print("dataset.csv generated")
