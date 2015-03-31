#pragma once
#include <math.h> //pow
#include <stdlib.h> //rand_r
#include <stdio.h> //printf
class zipf{
struct probvals {
  float prob;                  /* the access probability */
  float cum_prob;              /* the cumulative access probability */
};
struct probvals *zdist;         /* the probability distribution  */
public:
  zipf(float theta,int N){
    get_zipf(theta,N);
  }
void get_zipf(float theta, int N){
  float sum=0.0;
  float c=0.0;
  float expo;
  float sumc =0.0;
  int i;
  expo =  theta;
  zdist = (struct probvals *)malloc(N*sizeof(struct probvals));
  /*
  * zipfian - p(i) = c / i ^^ (1 - theta) At x
  * = 1, uniform * at x = 0, pure zipfian
  */
  for (i = 1; i <= N; i++) {
     sum += 1.0 /(float) pow((double) i, (double) (expo));
  }
  c = 1.0 / sum;

  //randomize
  //int result=rand_r(seed);
  for (i = 0; i < N; i++) {
    zdist[i].prob = c /(float) pow((double) (i + 1), (double) (expo));
  }

  //randomize
  //  int seed=1;
/*
  for(i=0;i<N*10;i++){
    int posA=rand()%N;
    int posB=rand()%N;
    float tmp = zdist[posA].prob;
    zdist[posA].prob=zdist[posB].prob;
    zdist[posB].prob=tmp;
  }
*/
  //endof random  
  for (i = 0; i < N; i++) {
    sumc +=  zdist[i].prob;
    zdist[i].cum_prob = sumc;
  }
}
int get_zipf_n(float prob,int N){
  //[left,right)
  int left=0;
  int right=N;
  while(true){
    if(left>=right-1){
      return left;
    }
    int middle=(left+right)/2;
    if(zdist[middle].cum_prob <= prob){
      left=middle;
    } else {
      right=middle;
    }
  }
}

int get_zipf_random(unsigned int* seed,int N){
  int result=rand_r(seed);
  int large_number=1<<30;
  return get_zipf_n((result%large_number)*1.0/large_number,N);
}

void print_first_n_element(int n){
  for(int i=0;i<n;i++){
    printf("%d,%f%%\n",i,zdist[i].prob*100);
  }
}
};
