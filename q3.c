/*In a warehouse management system, products are delivered and stored in a limited number of
  storage spaces. The warehouse has a limited capacity, and multiple delivery trucks can arrive
  simultaneously, each bringing a different quantity of products. To ensure efficient handling of
  incoming products and prevent overloading of the storage area, the system needs to manage
  the delivery process effectively.
  Implement a C program that simulates the inventory management of a warehouse. The program
  should consist of two types of threads: Delivery Trucks and Storage Managers.
  - Delivery Trucks: Each truck delivers a certain number of products to the warehouse. If
  the warehouse is full, the truck must wait until space is available.
  - Storage Managers: These workers are responsible for organizing and storing the
  products. If there are no products to store, they must wait until trucks arrive with new
  products.
  Design :
  1. Implement a circular buffer to represent the storage area in the warehouse. The buffer
  should have a maximum capacity that can be defined as a constant in the code.
  2. Create multiple delivery truck threads that simulate trucks arriving at the warehouse at
  random intervals, delivering a random number of products (between 1 and a predefined
  maximum).
  3. Create multiple storage manager threads that simulate the storage of products at
  random intervals, taking a random number of products from the warehouse (between 1
  and a predefined maximum) until the buffer is empty.
  You can use the following synchronization primitives: semaphores/mutexes to coordinate the
  access to the shared storage buffer and ensure that the delivery trucks and storage managers
  operate correctly without causing race conditions.
  The program should run for a defined period or until a certain number of deliveries have been
  processed, then print the final state of the warehouse.
  Inputs:
  - The number of products delivered must be randomly generated using rand() by Delivery
  Truck threads.
  - The number of products stored must be randomly generated using rand() by Storage
  Manager threads.
  - Buffer size must be a constant defined in your program.

  - The number of delivery trucks and storage managers must be passed as input to the
  program.

  Output:
  - Current Inventory Status: Messages printed to the console by the delivery truck and
  storage manager threads, indicating the current number of products in the buffer after
  each delivery and storage operation.
  - Buffer State Messages: Messages indicating attempts to add products to the buffer when
  it's full or attempts to take products when it's empty, which would help demonstrate
  synchronization issues and buffer management.*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 10      // capacity
#define MAX_PRODUCTS 5      // Max products per delivery 
#define MAX_DELIVERIES 20   // for stopping the program

int buf[BUFFER_SIZE];
int f = 0, r = 0, cnt = 0;  
int tot_deliv = 0;            // number of deliveries
int deliv_done = 0;         // end of deliveries

pthread_mutex_t mtx;
pthread_mutex_t deliv_mtx;    // for tot_deliv
sem_t full, empty;

void* delivery_truck(void* arg) {
    int trk_id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&deliv_mtx);
        if (tot_deliv >= MAX_DELIVERIES) {
            deliv_done = 1; 
            pthread_mutex_unlock(&deliv_mtx);

            int i = 0;
            while (i < BUFFER_SIZE) {
                sem_post(&full);
                sem_post(&empty);
                i++;
            }
            break;
        }
        pthread_mutex_unlock(&deliv_mtx);

        int prdcts = rand() % MAX_PRODUCTS + 1; 

        sem_wait(&empty); 
        
        pthread_mutex_lock(&mtx);

        if (cnt + prdcts <= BUFFER_SIZE) { 
            int i = 0;
            while (i < prdcts) {
                buf[r] = 1;  
                r = (r + 1) % BUFFER_SIZE;
                cnt++;
                i++;
            }

            pthread_mutex_lock(&deliv_mtx); 
            tot_deliv++;
            pthread_mutex_unlock(&deliv_mtx);

            printf("Truck %d delivered %d products. Current storage: %d\n", trk_id, prdcts, cnt);
        } else {
            int space = BUFFER_SIZE - cnt;
            printf("Truck %d waiting - Not enough space. Products to deliver: %d, Available space: %d\n", 
                   trk_id, prdcts, space);        }

        pthread_mutex_unlock(&mtx);
        sem_post(&full); 

        sleep(rand() % 3 + 1); 
    }

    return NULL;
}

void* storage_manager(void* arg) {
    int mgr_id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&deliv_mtx);
        if (deliv_done && cnt == 0) {
            pthread_mutex_unlock(&deliv_mtx);
            break; 
        }
        pthread_mutex_unlock(&deliv_mtx);

        int prdcts = rand() % MAX_PRODUCTS + 1; 

        sem_wait(&full); 
        pthread_mutex_lock(&mtx);
        if (cnt >= prdcts) { 
            int i = 0;
            while (i< prdcts) {
                buf[f] = 0;  
                f = (f + 1) % BUFFER_SIZE;
                cnt--;
                i++;
            }
            printf("Manager %d stored %d products. Current storage: %d\n", mgr_id, prdcts, cnt);
        } else {
                printf("Manager %d waiting - Not enough products (or empty) in the warehouse.\n", mgr_id);

        }

        pthread_mutex_unlock(&mtx);
        sem_post(&empty); 

        sleep(rand() % 3 + 1); 
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    int n_trks, n_mgrs;
    printf("Enter number of delivery trucks: ");
    scanf("%d", &n_trks);
    printf("Enter number of storage managers: ");
    scanf("%d", &n_mgrs);

    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_init(&deliv_mtx, NULL); 
    sem_init(&full, 0, 0);             
    sem_init(&empty, 0, BUFFER_SIZE);    

    pthread_t trks[n_trks], mgrs[n_mgrs];
    int trk_ids[n_trks], mgr_ids[n_mgrs];

    // Truck threads
    int i = 0;
    while (i < n_trks) {
        trk_ids[i] = i + 1;
        pthread_create(&trks[i], NULL, delivery_truck, &trk_ids[i]);
        i++;
    }

    // Manager threads
    i = 0;
    while (i < n_mgrs) {
        mgr_ids[i] = i + 1;
        pthread_create(&mgrs[i], NULL, storage_manager, &mgr_ids[i]);
        i++;
    }

    i = 0;
    while (i < n_trks) {
        pthread_join(trks[i], NULL);
        i++;
    }

    i= 0;
    while (i < n_mgrs) {
        pthread_join(mgrs[i], NULL);
        i++;
    }

    pthread_mutex_destroy(&mtx);
    pthread_mutex_destroy(&deliv_mtx);
    sem_destroy(&full);
    sem_destroy(&empty);
    printf("Completed the total delivery of %d products.\n", tot_deliv);
    return 0;
}
