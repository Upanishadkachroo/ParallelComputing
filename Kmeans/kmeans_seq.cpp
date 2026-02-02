#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>   // [ADDED] for timing
using namespace std;
using namespace std::chrono;  // [ADDED]

// Euclidean distance 
double euclideanDistance(const vector<double>& a, const vector<double>& b) {
    double sum = 0.0;
    //computes euclidean distance between data points and centroid
    for (int i = 0; i < a.size(); i++)
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(sum);
}

int main() {
    int N, K, D;

    // Input parameters
    cout << "Enter number of points: ";
    cin >> N;

    cout << "Enter number of clusters: ";
    cin >> K;

    cout << "Enter dimension: ";
    cin >> D;

    vector<vector<double>> data(N, vector<double>(D));

    cout << "Enter data points: " << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < D; j++) {
            cin >> data[i][j];
        }
    }

    // Fixed internal parameters 
    int maxIterations = 100;

    vector<vector<double>> centroids(K, vector<double>(D));
    vector<int> labels(N, -1);

    // Initialize centroids with first K points
    for (int i = 0; i < K; i++)
        centroids[i] = data[i];

    // [ADDED] Start execution time measurement
    auto startTime = high_resolution_clock::now();

    // K-Means Loop - controls convergence & stops when centroid no longer move
    for (int iter = 0; iter < maxIterations; iter++) {
        bool changed = false;

        // Assignment step - for each data point, it computes distance to all k cnetroids& assign the point to nearest centroid
        // O(N × K × D)
        for (int i = 0; i < N; i++) {
            double minDist = numeric_limits<double>::max();
            int cluster = -1;

            for (int j = 0; j < K; j++) {
                double dist = euclideanDistance(data[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    cluster = j;
                }
            }

            if (labels[i] != cluster) {
                labels[i] = cluster;
                changed = true;
            }
        }

        // Convergence check - if no points changed in cluster, the algo stops
        if (!changed)
            break;

        // Update centroids         
        vector<vector<double>> newCentroids(K, vector<double>(D, 0.0));
        vector<int> count(K, 0);

        // New centroid = Mean of all points in the cluster
        for (int i = 0; i < N; i++) {
            int c = labels[i];
            count[c]++;
            for (int d = 0; d < D; d++)
                newCentroids[c][d] += data[i][d];
        }

        for (int j = 0; j < K; j++) {
            for (int d = 0; d < D; d++)
                newCentroids[j][d] /= count[j];
        }

        centroids = newCentroids;
    }

    // [ADDED] Stop execution time measurement
    auto endTime = high_resolution_clock::now();
    duration<double> execTime = endTime - startTime;

    // Output 
    // cout << "\nCluster assignments:\n";
    // for (int i = 0; i < N; i++)
    //     cout << "Point " << i << " -> Cluster " << labels[i] << endl;

    cout << "\nFinal centroids:\n";
    for (int j = 0; j < K; j++) {
        cout << "Cluster " << j << ": ";
        for (int d = 0; d < D; d++)
            cout << centroids[j][d] << " ";
        cout << endl;
    }

    // [ADDED] Print execution time
    cout << "\nExecution Time (Sequential): "
         << execTime.count() << " seconds\n";

    return 0;
}


// 6 2 2
// 1.0 2.0
// 1.5 1.8
// 5.0 8.0
// 8.0 8.0
// 1.0 0.6
// 9.0 11.0
// 12.0 4.0