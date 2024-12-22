/*Write a multithreaded C program to simulate the Networked Servers Problem, a
  synchronization problem where servers are trying to access shared network resources. The
  goal is to ensure mutual exclusion and prevent resource starvation using Semaphores.
  Design:
  ● There are 5 servers in a data center, each trying to perform data processing tasks that
  require access to two network channels (left and right).
  ● Each network channel is shared by 2 adjacent servers. The last network channel is
  shared by the last server and the first server.
  ● A server can only process data when it has access to both its left and right network
  channels.
  ● Each network channel can only be used by one server at a time.
  ● Each server takes 1 second to do data processing. (You can use sleep() for this)
  ● The challenge is to design a solution where:
  1. Mutual exclusion is maintained (no two servers can access the same network
  channel simultaneously).
  2. Starvation is prevented (every server should get a chance to access the network
  channels).

  Use Semaphores to represent the network channels and ensure proper synchronization among
  the servers.
  1. Input:
  ○ None
  2. Output:
  ○ Each server should print messages indicating when they are waiting (attempting
  to acquire network channels), and processing (after acquiring both channels).
  ○ The program should terminate after each server has processed data 3 times.
  Example:
  Server 1 is processing
  Server 2 is waiting
  Server 4 is waiting

  Server 2 is processing
  Server 1 is waiting*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t c1, c2, c3, c4, c5; // Semaphores as channels
int is_channel_busy(sem_t *channel) {
    int value;
    sem_getvalue(channel, &value);
    return value == 0; // if the channel is busy
}
void *server(void *arg) {
    int ser_num = *(int *)arg;

    // left and right channels
    sem_t *left_channel = (ser_num == 0) ? &c1 : (ser_num == 1) ? &c2 : (ser_num == 2) ? &c3 : (ser_num == 3) ? &c4 : &c5;
    sem_t *right_channel = (ser_num == 0) ? &c2 : (ser_num == 1) ? &c3 : (ser_num == 2) ? &c4 : (ser_num == 3) ? &c5 : &c1;

    int count = 0;
    while (count < 3) {
        // Wait for both channels to become available
     if (is_channel_busy(left_channel)|| is_channel_busy(right_channel)) {
                printf("Server %d is waiting \n", ser_num + 1);
            }
        
        //  fixed order to avoid deadlock
        if (left_channel < right_channel) {
            sem_wait(left_channel);
            sem_wait(right_channel);
        } else {
            sem_wait(right_channel);
            sem_wait(left_channel);
        }

        
        printf("Server %d is processing\n", ser_num + 1);
        sleep(1); //  processing time

        printf("Server %d has finished processing\n", ser_num + 1);
        sem_post(left_channel);
        sem_post(right_channel);

        count++;

        usleep(100000); // Small delay before retrying
    }
    return NULL;
}

int main() {
   
    sem_init(&c1, 0, 1);
    sem_init(&c2, 0, 1);
    sem_init(&c3, 0, 1);
    sem_init(&c4, 0, 1);
    sem_init(&c5, 0, 1);

    pthread_t servers[5];
    int ser_num[5] = {0, 1, 2, 3, 4};

    
    int i = 0;
    while (i < 5) {
        pthread_create(&servers[i], NULL, server, &ser_num[i]);
        i++;
    }
    
    i = 0;  
    while (i < 5) {
        pthread_join(servers[i], NULL);
        i++;
    }


    sem_destroy(&c1);
    sem_destroy(&c2);
    sem_destroy(&c3);
    sem_destroy(&c4);
    sem_destroy(&c5);

    printf("All servers have completed processing.\n");
    return 0;
}



