#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){
    int queue[10];
    int index=0;

    #pragma omp parallel
    {
        //give unique ids to each thread entering parallel section
        int id=omp_get_thread_num();

        //race condition
        // queue[index]=id;
        // index++;


        //fix using critical
        #pragma omp critical
        {
            queue[index]=id;
            index++;
        }
    }

    for(int i=0; i<index; i++){
        cout<<queue[i]<<" ";
    }
}