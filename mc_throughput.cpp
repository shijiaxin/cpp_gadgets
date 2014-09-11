//g++ -lpthread mc_throughput.cpp -o mc_throughput

#include <unistd.h>    // for sleep
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <string.h>

#include "timer.h"

//variables used to calculate throughput
typedef unsigned long uint64_t;
pthread_t th[16];
int epoch=0;
int thrnum=2;
int ready=0;
pthread_mutex_t print_mutex;

//varibales related with workload
#define ARRAYSIZE 4*1024*1024/CACHELINESIZE //4M
#define CACHELINESIZE 64 //64 bytes
struct Cacheline {
    char data[CACHELINESIZE];
};


Cacheline * array;
pthread_spinlock_t * lockarray;

void global_init(){
    array = (Cacheline *)malloc(ARRAYSIZE * sizeof(Cacheline));    
    memset(array, 0, ARRAYSIZE * sizeof(Cacheline)); 
    lockarray=(pthread_spinlock_t *) malloc(ARRAYSIZE * sizeof(pthread_spinlock_t));
    for(int i=0;i<ARRAYSIZE;i++){
        pthread_spin_init(&lockarray[i], 0);
    }
    
    pthread_mutex_init(&print_mutex, NULL);
}

void* thread_body(void *x) {

    int lepoch = 0;        
    long tid = (long)x;
    int cpu = tid;

    /// set affinity
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);

    // thread_init(); if necessary    
    __sync_fetch_and_add(&ready, 1);
    
    while(epoch == 0);
    lepoch = epoch;
    
    int loop=0;
    int count=0;            
    timer t1,t2;

    unsigned int rseed=tid;
    while(true) {
        /// do some work        
        {
            uint64_t n=rand_r(&rseed) % ARRAYSIZE;
            pthread_spin_lock(  &lockarray[n]);
            for(int i=0;i<64;i++){
                count+=array[n].data[i];
                array[n].data[i]++;
            }
            pthread_spin_unlock(&lockarray[n]);
            //int a = rand();
        }
        loop++;
        if(lepoch < epoch) {
            // report and reset
            t2.reset();
            pthread_mutex_lock (&print_mutex);
            std::cout<<"tid \t"<<tid<<"\t"
                     <<"loop\t"<<loop<<"\t"
                     <<"time\t"<<t2.diff(t1)<<std::endl;
            pthread_mutex_unlock (&print_mutex);

            while(epoch%2 ==0) ;

            t1.reset();
            lepoch = epoch;
            loop=0;
        }

    } 
}

int main(int argc, char** argv) {
    


    global_init();    
    for(int i = 0; i < thrnum; i++)
        pthread_create(&th[i], NULL, thread_body, (void *)i);

    //Barriar to wait all threads become ready
    while (ready < thrnum);
    //Begin at the first epoch
    epoch = 1;
    sleep(5); //for warmup
    epoch++;
    sleep(0.5);
    epoch++;
    while(true) {
        sleep(5);
        epoch++;
        sleep(0.5);
        epoch++;
    }
    for(int i = 0; i < thrnum; i++)
        pthread_join(th[i], NULL);
    
    
    return 1;
}