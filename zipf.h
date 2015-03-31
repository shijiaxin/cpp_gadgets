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
int N;
public:
  zipf(float theta,int N){
    get_zipf(theta,N);
  }
void get_zipf(float theta, int n){
  float sum=0.0;
  float c=0.0;
  float expo;
  float sumc =0.0;
  int i;
  expo =  theta;
  N=n;
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
int get_zipf_n(double prob){
  //find first number who > prob;
  int left=0;
  int right=N;
  if(prob<zdist[0].cum_prob)
    return 0;
  int pos=1;
  while(pos<N){
    if( zdist[pos].cum_prob>prob)
      return pos;
    int inc=1;
    while((pos+2*inc<N) && (zdist[pos+2*inc].cum_prob<prob)){
        inc*=2;
    }
    pos+=inc;
  }
  return N-1;
}

int get_zipf_random(unsigned int* seed){
  double prob=(double)rand()/RAND_MAX;
  return get_zipf_n(prob);
}

void print_first_n_element(int n){
  for(int i=0;i<n;i++){
    printf("%d,%f%%\n",i,zdist[i].prob*100);
  }
}
};
