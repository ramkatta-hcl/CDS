/*thread_synchronization_uisng_mutex_condwait */

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
