#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

struct Point { double x, y; };

double dist2(const Point &a, const Point &b){
    double dx=a.x-b.x, dy=a.y-b.y;
    return dx*dx + dy*dy;
}

int main(int argc, char** argv){

    if(argc < 5){
        cout << "Usage: ./kmeans_openmp dataset.csv K max_iters num_threads\n";
        return 0;
    }

    string filename = argv[1];
    int K = stoi(argv[2]);
    int max_iters = stoi(argv[3]);
    int threads = stoi(argv[4]);

    omp_set_num_threads(threads);

    // -------- Read dataset --------
    vector<Point> pts;
    ifstream fin(filename);
    string line;

    getline(fin, line);        // skip header
    while(getline(fin, line)){
        if(line.empty()) continue;
        replace(line.begin(), line.end(), ',', ' ');
        stringstream ss(line);
        Point p; ss >> p.x >> p.y;
        pts.push_back(p);
    }
    fin.close();

    int N = pts.size();

    // -------- Initialize centroids --------
    vector<Point> cent(K);
    for(int i=0;i<K;i++)
        cent[i] = pts[(i*N)/K];

    vector<int> label(N, -1);

    auto t1 = chrono::high_resolution_clock::now();

    // ======== K MEANS ITERATIONS ========
    for(int it=0; it<max_iters; it++){

        vector<vector<double>> sumx(threads, vector<double>(K,0));
        vector<vector<double>> sumy(threads, vector<double>(K,0));
        vector<vector<int>> cnt(threads, vector<int>(K,0));

        bool changed = false;

        #pragma omp parallel
        {
            int tid = omp_get_thread_num();

            #pragma omp for
            for(int i=0;i<N;i++){

                double best = 1e18;
                int best_k = 0;

                for(int j=0;j<K;j++){
                    double d = dist2(pts[i], cent[j]);
                    if(d < best){
                        best = d;
                        best_k = j;
                    }
                }

                if(label[i] != best_k) changed = true;

                label[i] = best_k;

                sumx[tid][best_k] += pts[i].x;
                sumy[tid][best_k] += pts[i].y;
                cnt[tid][best_k]  += 1;
            }
        }

        // ----- reduce -----
        vector<double> gx(K,0), gy(K,0);
        vector<int> gc(K,0);

        for(int t=0;t<threads;t++){
            for(int j=0;j<K;j++){
                gx[j]+=sumx[t][j];
                gy[j]+=sumy[t][j];
                gc[j]+=cnt[t][j];
            }
        }

        // ----- update centroids -----
        for(int j=0;j<K;j++){
            if(gc[j] > 0){
                cent[j].x = gx[j]/gc[j];
                cent[j].y = gy[j]/gc[j];
            }
        }

        if(!changed) break;
    }

    auto t2 = chrono::high_resolution_clock::now();

    double time =
        chrono::duration<double>(t2-t1).count();

    cout<<"TIME_SECONDS "<<time<<endl;

    return 0;
}
