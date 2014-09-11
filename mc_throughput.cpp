//g++ -lpthread mc_throughput.cpp -o mc_throughput

#include <unistd.h>    // for sleep
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "timer.h"

//variables used to calculate throughput
pthread_t th[64];
int epoch=0;
int thrnum=2;
int ready=0;


//varibales related with workload


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
    timer t1,t2;

    while(true) {
        /// do some work        
        {
        int count=0;
            for(int i=0;i<1000000;i++){
                count+=i;
            }                    
        }
        loop++;
        if(lepoch < epoch) {
            // report and reset
            t2.reset();
            std::cout<<"tid \t"<<tid<<"\t"
                     <<"loop\t"<<loop<<"\t"
                     <<"time\t"<<t2.diff(t1)<<std::endl;
            t1.reset();
            lepoch = epoch;
            loop=0;
        }
    } 
}

int main(int argc, char** argv) {
    

    for(int i = 0; i < thrnum; i++)
        pthread_create(&th[i], NULL, thread_body, (void *)i);


    //Barriar to wait all threads become ready
    while (ready < thrnum);
    //Begin at the first epoch
    epoch = 1;
    sleep(5); //for warmup
    epoch++;    
    while(true) {
        sleep(5);
        epoch++;
    }
    for(int i = 0; i < thrnum; i++)
        pthread_join(th[i], NULL);
    
    
    return 1;
}