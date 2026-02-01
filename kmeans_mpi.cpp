// mpic++ kmeans_mpi.cpp -o kmeans_mpi
// mpirun -np 4 ./kmeans_mpi

// MPI follows the SPMD(Single Pocess, Multiple Data) model,
// Same program runs on all processes, but behavior depends on rank.

#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

double euclideanDistance(const vector<double>& a, const vector<double>& b) {
    double sum = 0.0;
    for (int i = 0; i < a.size(); i++)
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(sum);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N, K, D;

    // Only rank 0 reads input
    if (rank == 0)
        cin >> N >> K >> D;

    // MPI_Bcast ensures consistent global configuration.
    // tell other processes - no. of points, no. of clusters & data dimension
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&D, 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<double> fullData;
    if (rank == 0) {
        fullData.resize(N * D);
        for (int i = 0; i < N * D; i++)
            cin >> fullData[i];
    }

    int localN = N / size;
    vector<double> localData(localN * D);

    // Full dataset exists only on rank 0 - Data is split evenly (Data Parallelism)
    MPI_Scatter(fullData.data(), localN * D, MPI_DOUBLE,
                localData.data(), localN * D, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    vector<double> centroids(K * D);
    // Initial centroids chosen on rank 0, Broadcast to all processes
    if (rank == 0)
        for (int i = 0; i < K * D; i++)
            centroids[i] = fullData[i];

    MPI_Bcast(centroids.data(), K * D, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<int> labels(localN);
    int maxIterations = 100;

    // [ADDED] Synchronize all processes before timing
    MPI_Barrier(MPI_COMM_WORLD);
    double startTime = MPI_Wtime();   // [ADDED]

    // This loop represents one global K-Means iteration.
    for (int iter = 0; iter < maxIterations; iter++) {
        vector<double> localSum(K * D, 0.0);
        vector<int> localCount(K, 0);

        // for each process - Works only on its local data &
        // Assigns each local point to nearest centroid
        // O((N/P) × K × D)
        for (int i = 0; i < localN; i++) {
            double minDist = numeric_limits<double>::max();
            int best = -1;

            for (int j = 0; j < K; j++) {
                double dist = 0.0;
                for (int d = 0; d < D; d++) {
                    double diff = localData[i * D + d] - centroids[j * D + d];
                    dist += diff * diff;
                }

                if (dist < minDist) {
                    minDist = dist;
                    best = j;
                }
            }

            labels[i] = best;
            localCount[best]++;
            for (int d = 0; d < D; d++)
                localSum[best * D + d] += localData[i * D + d];
        }

        vector<double> globalSum(K * D);
        vector<int> globalCount(K);

        // Combines local results from all processes &&
        // Distributes global sums back to all processes
        MPI_Allreduce(localSum.data(), globalSum.data(),
                      K * D, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(localCount.data(), globalCount.data(),
                      K, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        for (int j = 0; j < K; j++)
            for (int d = 0; d < D; d++)
                centroids[j * D + d] = globalSum[j * D + d] / globalCount[j];
    }

    // [ADDED] Stop timing after computation
    MPI_Barrier(MPI_COMM_WORLD);
    double endTime = MPI_Wtime();     // [ADDED]
    double totalTime = endTime - startTime; // [ADDED]

    // Gather final cluster labels from all processes to rank 0
    vector<int> allLabels;
    if (rank == 0)
        allLabels.resize(N);

    MPI_Gather(labels.data(), localN, MPI_INT,
               allLabels.data(), localN, MPI_INT,
               0, MPI_COMM_WORLD);

    // Output results (similar to sequential code)
    if (rank == 0) {
        cout << "\nFinal Centroids:\n";
        for (int j = 0; j < K; j++) {
            cout << "Cluster " << j << ": ";
            for (int d = 0; d < D; d++)
                cout << centroids[j * D + d] << " ";
            cout << endl;
        }

        // [ADDED] Print total execution time
        cout << "\nExecution Time (MPI Parallel): "
             << totalTime << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}

// 6 2 2
// 1.0 2.0
// 1.5 1.8
// 5.0 8.0
// 8.0 8.0
// 1.0 0.6
// 9.0 11.0
