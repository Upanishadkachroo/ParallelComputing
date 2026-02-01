// g++ -fopenmp kmeans_openmp.cpp -o kmeans_openmp

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <omp.h>

using namespace std;

double euclideanDistance(const vector<double>& a,
                         const vector<double>& b) {
    double sum = 0.0;
    for (int i = 0; i < a.size(); i++)
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(sum);
}

int main() {
    int N, K, D;
    cin >> N >> K >> D;

    vector<vector<double>> data(N, vector<double>(D));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < D; j++)
            cin >> data[i][j];

    int maxIterations = 100;

    vector<vector<double>> centroids(K, vector<double>(D));
    vector<int> labels(N, -1);

    for (int i = 0; i < K; i++)
        centroids[i] = data[i];

    for (int iter = 0; iter < maxIterations; iter++) {
        bool changed = false;

        /* Assignment step */
        #pragma omp parallel for reduction(||:changed)
        for (int i = 0; i < N; i++) {
            double minDist = numeric_limits<double>::max();
            int best = -1;

            for (int j = 0; j < K; j++) {
                double dist = euclideanDistance(data[i], centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    best = j;
                }
            }

            if (labels[i] != best) {
                labels[i] = best;
                changed = true;
            }
        }

        if (!changed) break;

        vector<vector<double>> newCentroids(K, vector<double>(D, 0.0));
        vector<int> count(K, 0);

        /* Centroid update */
        #pragma omp parallel
        {
            vector<vector<double>> localCentroids(K, vector<double>(D, 0.0));
            vector<int> localCount(K, 0);

            #pragma omp for
            for (int i = 0; i < N; i++) {
                int c = labels[i];
                localCount[c]++;
                for (int d = 0; d < D; d++)
                    localCentroids[c][d] += data[i][d];
            }

            #pragma omp critical
            {
                for (int j = 0; j < K; j++) {
                    count[j] += localCount[j];
                    for (int d = 0; d < D; d++)
                        newCentroids[j][d] += localCentroids[j][d];
                }
            }
        }

        for (int j = 0; j < K; j++)
            for (int d = 0; d < D; d++)
                centroids[j][d] = newCentroids[j][d] / count[j];
    }

    for (int i = 0; i < N; i++)
        cout << "Point " << i << " -> Cluster " << labels[i] << endl;

    return 0;
}

