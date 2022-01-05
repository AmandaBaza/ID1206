#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "green.h"

struct timeval stop, start;
double secs = 0;
int flag = 0 ;
green_cond_t cond ;
green_mutex_t mutex;
int l = 100000;

void *test1(void *arg){
    int i = *(int*) arg ;
    int loop = 4;
    while( loop > 0){
        printf ( "thread %d : %d\n" , i , loop ) ;
        loop --;
        green_yield();
    }
}

void * test2 ( void * arg ) {
    int id = *( int *) arg ;
    int loop = 4;

    while ( loop > 0 ) {
        if( flag == id ) {
            printf ("thread %d : %d\n" , id , loop);
            loop --;
            flag = ( id + 1 ) % 2 ;
            green_cond_signal (&cond) ;
        }else{
            green_cond_wait(&cond, &mutex) ;
        }
    }
}

void * test3 ( void * arg ) {
    int id = *( int *) arg ;
    int loop = l*2;

    while ( loop > 0 ) {
        green_mutex_lock(&mutex) ;
        while ( flag != id ) {
            //green_mutex_unlock(&mutex);
            green_cond_wait(&cond, &mutex);
            //green_mutex_lock(&mutex);
        }
        if(flag + 1 < l){
            flag++;
        }else{
            flag = 0;
        }
        //flag = ( id + 1 ) % 2 ;
        green_cond_signal(&cond);
        green_mutex_unlock(&mutex);
        loop--;
    }
}

void * test4 ( void * arg ) {
    
    for(int i = 0; i < l; i++){
       ; 
    }
}

void * test ( void * arg ) {
    int id = *( int *) arg ;
    int loop = 4;
    while( loop > 0){
        printf ( "thread %d : %d\n" , id , loop ) ;
        loop --;
        green_yield();
    }
    printf("Test 1 done\n");
    loop = 4;
    while ( loop > 0 ) {
        if( flag == id ) {
            printf ("thread %d : %d\n" , id , loop);
            loop --;
            flag = ( id + 1 ) % 2 ;
            green_cond_signal (&cond) ;
        }else{
            green_cond_wait(&cond, &mutex) ;
        }
    }
    loop = 4;
   
    printf("Test 2 done\n");
    while ( loop > 0 ) {
        green_mutex_lock(&mutex) ;
        while ( flag != id ) {
            //green_mutex_unlock(&mutex);
            green_cond_wait(&cond, &mutex);
            //green_mutex_lock(&mutex);
        }
        flag = ( id + 1 ) % 2 ;
        green_cond_signal(&cond);
        green_mutex_unlock(&mutex);
        loop--;
    } printf("Test 3 done\n");
}

int main () {
    green_t g[l];
    green_t g0,g1;
    pthread_t p[l];
    int a[l];
    int a0 = 0;
    int a1 = 1;
    for(int i = 0;i<l;i++){
        a[i]=i;
    }
    
    gettimeofday(&start, NULL);
    for(int i = 0; i < l; i++){
        green_create(&g[i] , test4, &a[i]);
    }
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time loop create(s): %f\n",secs);
    //printf("%f, ", secs);

    gettimeofday(&start, NULL);
    for(int i = 0; i < l; i++){
        green_join(&g[i] , NULL);
    }
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time loop join(s): %f\n",secs);
    //printf("%f\n", secs);

    //----------------------------
    gettimeofday(&start, NULL);
    for(int i = 0; i < l; i++){
        pthread_create(&p[i], NULL, test4 ,NULL);
    }
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time loop pthread(s): %f\n",secs);
    //printf("%f, ", secs);

    gettimeofday(&start, NULL);
    for(int i = 0; i < l; i++){
        pthread_join(p[i] , NULL);
    }
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time loop pthread join(s): %f\n",secs);
    //printf("%f\n", secs);
    
    /*
    gettimeofday(&start, NULL);
    green_create(&g0 , test3, &a0);
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time c1(s): %f\n",secs);

    gettimeofday(&start, NULL);
    green_create(&g1 , test3, &a1);
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time c1(s): %f\n",secs);

    gettimeofday(&start, NULL);
    green_join(&g0 , NULL);
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time j0(s): %f\n",secs);
    
    gettimeofday(&start, NULL);
    green_join(&g1 , NULL);
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Time j1(s): %f\n",secs);
    */
    
    //printf("done\n");
    return 0 ;
}
