#include "timer.h"
#include <iostream>
using namespace std;

int main(){
    timer t1;
    int count=0;
    for(int i=0;i<100000000;i++){
        count+=i;
    }
    timer t2;
    cout<<"count\t"<<count<<endl
        <<"time \t"<<t2.diff(t1)<<endl;

}

