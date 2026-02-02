import numpy as np
import sys

if len(sys.argv) < 3:
    print("Usage: python3 generate_matrix.py N output_prefix")
    sys.exit(0)

N = int(sys.argv[1])
prefix = sys.argv[2]

A = np.random.randint(0, 10, (N, N))
B = np.random.randint(0, 10, (N, N))

np.savetxt(prefix + "_A.csv", A, delimiter=",", fmt="%d")
np.savetxt(prefix + "_B.csv", B, delimiter=",", fmt="%d")

print(f"Generated {N}x{N} matrices: {prefix}_A.csv , {prefix}_B.csv")


#python3 generate_matrix.py 800 mat800
