#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
struct Point { double x, y; };

double dist2(const Point &a, const Point &b){
    double dx=a.x-b.x, dy=a.y-b.y;
    return dx*dx + dy*dy;
}

vector<Point> readCSV(string file){
    vector<Point> pts;
    ifstream fin(file);
    string line;

    getline(fin, line);
    while(getline(fin, line)){
        if(line.empty()) continue;
        replace(line.begin(), line.end(), ',', ' ');
        stringstream ss(line);
        Point p; ss>>p.x>>p.y;
        pts.push_back(p);
    }
    fin.close();
    return pts;
}

int main(int argc, char** argv){

    MPI_Init(&argc,&argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    //check this
    //input validation + safe MPI exit
    if(argc < 4){
        if(rank==0)
            //Only master process prints the message.
            cout<<"Usage: mpirun -np P ./kmeans_mpi data.csv K max_iters\n";
        MPI_Finalize();
        return 0;
    }

    string filename = argv[1];
    int K = stoi(argv[2]);
    int max_iters = stoi(argv[3]);

    vector<Point> pts = readCSV(filename);
    int N = pts.size();

    vector<Point> cent(K);

    //check this
    //only the master process (rank 0) performs initialization& evry process will have its own pts[]
    if(rank==0){
        for(int i=0;i<K;i++)
            cent[i] = pts[(i*N)/K]; //Pick K initial points evenly spaced from dataset
    }

    // rank 0 sends data to all process
    MPI_Bcast(cent.data(), K*sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);

    //time start
    double t1 = MPI_Wtime();
    
    //check for max iteration
    for(int it=0; it<max_iters; it++){

        vector<double> sumx(K,0), sumy(K,0);
        vector<int> cnt(K,0);

        // each process handles strided part, splits data
        for(int i=rank;i<N;i+=size){
            //find nearest point i.e centroid
            double best=1e18;
            int best_k=0;

            for(int j=0;j<K;j++){
                //function call for eucledian distance
                double d = dist2(pts[i], cent[j]);
                if(d < best){
                    best=d;
                    best_k=j;
                }
            }

            sumx[best_k]+=pts[i].x;
            sumy[best_k]+=pts[i].y;
            cnt[best_k]++;
        }

        vector<double> gx(K), gy(K);
        vector<int> gc(K);

       //distributed reduction
       //sum of all sumx from all ranks: gx
        MPI_Allreduce(sumx.data(), gx.data(), K, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(sumy.data(), gy.data(), K, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(cnt.data(),  gc.data(), K, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        for(int j=0;j<K;j++){
            if(gc[j]>0){
                cent[j].x = gx[j]/gc[j];
                cent[j].y = gy[j]/gc[j];
            }
        }

        // All processes must have same centroids next iteration
        MPI_Bcast(cent.data(), K*sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);
    }

    double t2 = MPI_Wtime();

    if(rank==0)
        cout<<"TIME_SECONDS "<<(t2-t1)<<endl;

    MPI_Finalize();
}
