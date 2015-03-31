#include "zipf.h"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
struct mystruct{
  int index;
  int value;
  mystruct(){
    index=0;
    value=0;
  }
  mystruct(int i,int v){
    index=i;
    value=v;
  }
};
bool mygreater(const mystruct& a,const mystruct& b){
    return a.value>b.value;
}
int main(){
    zipf dist(0.99,1000);
    cout<<"printing probability ..."<<endl;
    dist.print_first_n_element(20);
    vector<mystruct> result(1000);
    for(int i=0;i<result.size();i++){
        result[i].index=i;
        result[i].value=0;
    }
    unsigned int seed=0;
    int total=1000000;
    for(int i=0;i<total;i++){
        result[dist.get_zipf_random(&seed)].value++;
    }
    sort(result.begin(),result.end(),mygreater);
    cout<<"printing simulate frequence ..."<<endl;
    for(int i=0;i<20;i++){
        cout<<result[i].index<<","<<result[i].value*100.0/total<<"%"<<endl;
    }
}
