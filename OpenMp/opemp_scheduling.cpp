#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

void test(string type){
    cout<<type<<" scheduling"<<endl;

    #pragma omp parallel for schedule(runtime)
    for(int i=0; i<12; i++)
    {
        #pragma omp critical
        {
            cout<<"iteration "<<i<<" THread is ->"<<omp_get_thread_num()<<endl;
        }
    }
}

int main(){
    omp_set_num_threads(4);

    //static
    //iterations are divided beforehand, pre divided chunks, no runtime decison, least overhead
    omp_set_schedule(omp_sched_static, 0);
    test("static");

    //dynamic
    //threads take next avilble iteration, load balanced, more overhead
    omp_set_schedule(omp_sched_dynamic, 1);
    test("dynamic");

    //guided
    //large chunks at start, smaller later
    omp_set_schedule(omp_sched_guided, 2);
    test("guided");
}