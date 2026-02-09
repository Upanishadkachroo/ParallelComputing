#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){

    #pragma omp parallel for ordered
    for(int i=0; i<4; i++){
        //some parallel task

        #pragma omp ordered
        {
            cout<<"Thread"<<omp_get_thread_num()<<" prints index"<<i<<endl;
        }
    }
}