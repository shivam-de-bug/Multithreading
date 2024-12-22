/*Write a C program to simulate thread scheduling that ensures deadlock avoidance using a
  simplified resource allocation system. The goal is to avoid deadlock by controlling the order in
  which threads acquire locks, based on a predefined global knowledge of resources.
  Design:
  ● You should create 3 threads (T1, T2, T3), and each thread needs to acquire 2 locks
  (Lock A and Lock B) in a specific order.
  ● The threads should not be able to acquire locks in a fashion such that the program
  deadlocks.
  ● The program should ensure deadlock-free execution by scheduling the acquisition of
  locks based on the given deadlock avoidance algorithm.
  ● Implement deadlock avoidance using Resource Instance Ordering as follows:
  ○ Enforce a strict lock acquisition order (e.g., Lock A must always be acquired
  before Lock B).

  1. Input:
  ○ None
  2. Output:
  ○ Print messages showing the order of lock acquisition and thread waiting (e.g.,
  "T1 acquired Lock A", "T2 waiting for Lock A").
  ○ The program should terminate after each thread has acquired each lock 3 times.*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lockA;
pthread_mutex_t lockB;

void* locking(void* arg) {
    int thread_num = *(int*)arg;
    const char* tname = thread_num == 0 ? "T1" : (thread_num == 1 ? "T2" : "T3");

    int i = 0;  
    while (i < 3) { 
        //Lock A first, then Lock B
        printf("%s is attempting to acquire Lock A\n", tname);
        pthread_mutex_lock(&lockA);
        printf("%s acquired Lock A\n", tname);

        printf("%s is attempting to acquire Lock B\n", tname);
        pthread_mutex_lock(&lockB);
        printf("%s acquired Lock B\n", tname);

        printf("%s is in critical section\n", tname);

        pthread_mutex_unlock(&lockB);
        printf("%s released Lock B\n", tname);

        pthread_mutex_unlock(&lockA);
        printf("%s released Lock A\n", tname);

        usleep(100);  
        i++; 
    }

    return NULL;
}

int main() {
    
    pthread_mutex_init(&lockA, NULL);
    pthread_mutex_init(&lockB, NULL);

    pthread_t t1, t2, t3;
    int t1_id = 0, t2_id = 1, t3_id = 2;

   
    pthread_create(&t1, NULL, locking, &t1_id);
    pthread_create(&t2, NULL,locking, &t2_id);
    pthread_create(&t3, NULL, locking, &t3_id);

   
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

   
    pthread_mutex_destroy(&lockA);
    pthread_mutex_destroy(&lockB);

    printf("All threads completed successfully.\n");
    return 0;
}
