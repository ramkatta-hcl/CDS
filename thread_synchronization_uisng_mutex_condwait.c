/*thread_synchronization_uisng_mutex_condwait */
/*thread 1 prints odd num's and thread 2 prints even num's and the final o/p should come in order ex: 1,2,3,4..*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex_lock;
int count = 0;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

void *thread_function1() {
   pthread_mutex_lock(&mutex_lock);
   while(count<20) {
       if (count%2 == 0) {
           pthread_cond_wait(&condition_var, &mutex_lock);
       } else {
           printf("thread1: %d\n", count);
           count++;
           pthread_cond_signal(&condition_var);
           pthread_mutex_unlock(&mutex_lock);
       }
   }
   pthread_exit(NULL);
}

void *thread_function2() {
    pthread_mutex_lock(&mutex_lock);
    while(count<20) {
        if (count%2 != 0) {
            pthread_cond_wait(&condition_var, &mutex_lock);
        } else {
            printf("thread2: %d\n", count);
            count++;
            pthread_cond_signal( &condition_var );
            pthread_mutex_unlock(&mutex_lock);
        }
    }
    pthread_exit(NULL);
}

int main() {
   pthread_t thread_id[1];
   int ret;
   void *thread_result;

    ret=pthread_mutex_init(&mutex_lock, NULL);
   if(ret!=0) {
       perror("failed to initialize mutex\n");
       return;
   }

    ret = pthread_create(&thread_id[0], 0, thread_function1, NULL);
   if (ret != 0) {
       perror("thread1 creation failed\n");
       return;
   }

    ret = pthread_create(&thread_id[1], 0, thread_function2, NULL);
   if (ret !=0) {
       perror("thread2 creation failed\n");
       return;
   }

   printf("in main waiting for thread1 join\n");
   ret = pthread_join(thread_id[0], NULL);
   if (ret != 0) {
       perror("Failed to join thread1\n");
   }

    printf("in main waiting for thread2 to join\n");
   ret = pthread_join(thread_id[1], NULL);
   if(ret != 0) {
       perror("failed to join thread2\n");
   }

    pthread_mutex_destroy(&mutex_lock);

}

output:

bgl-ads-938:677> gcc thread_synchronization_uisng_mutex_condwait.c -lpthread
bgl-ads-938:678> 
bgl-ads-938:678> 
bgl-ads-938:678> ./a.out 
in main waiting for thread1 join
thread2: 0
thread1: 1
thread2: 2
thread1: 3
thread2: 4
thread1: 5
thread2: 6
thread1: 7
thread2: 8
thread1: 9
thread2: 10
thread1: 11
thread2: 12
thread1: 13
thread2: 14
thread1: 15
thread2: 16
thread1: 17
thread2: 18
thread1: 19
in main waiting for thread2 to join
bgl-ads-938:679>
