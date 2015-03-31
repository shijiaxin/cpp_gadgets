#include<algorithm> //sort
#include<iostream>
using namespace std;

bool my_less(const int a,const int b)
{
    return (a)*(a) < (b)*(b);
}
int main(){
  //simplest sort(void* begin,void* end)
  int array1[10]={1,10,2,9,3,8,4,7,5,6};
  sort(array1,array1+10);
  cout<<"sorting array1 ... ..."<<endl;
  for(int i=0;i<10;i++)
    cout<<array1[i]<<endl;

  //user-defined-sort 
  cout<<"sorting array2 ... ..."<<endl;
  int array2[10]={-1,10,-2,3,9,-8,-4,7,5,-6};
  sort(array2,array2+10,my_less);
  for(int i=0;i<10;i++)
    cout<<array2[i]<<endl;

}
