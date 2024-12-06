/* Author: Dana Maloney
   Operating Systems FA24 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER_SIZE 10 
int buffer[BUFFER_SIZE]; 
int buffer_items = 0;  // # items in buffer
//int next_item = 0;  // next item for producer.

//semaphores for buffer status:
sem_t empty;  // track empty slots
sem_t full;   // track full slots
pthread_mutex_t buffer_mutex;  // Mutex to protect buffer access

void *provider(void *arg) {
    /*  Provider Function:
        Come up with integers to get bought. */

    //seed random num generator
    srand(pthread_self());

    while (1) {
        //int item = next_item++;  // Produce an item (just a sequential integer here)
        int item = (rand() % 100) + 1; // come up with integer 1 to 100.
        sem_wait(&empty);  // Wait if no empty slot in buffer
        pthread_mutex_lock(&buffer_mutex);  // Lock buffer access

        // Add item to the buffer
        buffer[buffer_items++] = item;
        printf("Provider produced: %d\n", item);

        pthread_mutex_unlock(&buffer_mutex);  // Unlock buffer access
        sem_post(&full);  // Signal that a new item is available in buffer

        sleep(1);  // Simulate delay in production
    }
    return NULL;
}

// Consumer function for each buyer thread
void *buyer(void *arg) {
    int buyer_id = *(int *)arg; // ID number that's human readable.

    srand(pthread_self()); // seed random number.

    while (1) {
        sem_wait(&full);  // Wait if no items in buffer.
        pthread_mutex_lock(&buffer_mutex);  // Access buffer.

        // Consume item from the buffer
        int item = buffer[--buffer_items];
        printf("Buyer %d bought: %d\n", buyer_id, item);

        pthread_mutex_unlock(&buffer_mutex);  // Release buffer.
        sem_post(&empty); // indicate open spot in buffer.

        sleep(rand() % 5 + 1);  // 1 - 5 Second delay.
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Please enter one integer argument for the number of \'buyer\' threads.");
        return 1;
    }

    int num_buyers = atoi(argv[1]);
    if (num_buyers < 1) {
        fprintf(stderr, "Please give a number >= 1 for the number of buyers.\n");
        return 1;
    }

    pthread_t provider_thread; // single provider thread
    pthread_t buyers[num_buyers]; // array of buyer threads
    int buyer_id_num[num_buyers];

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);

    // Create threads.
    pthread_create(&provider_thread, NULL, provider, NULL); // Provider thread

    //create consumers with unique IDs. (buyer_id_num is an array so ints can be passed thru as pointers)
    for (int i = 0; i < num_buyers; i++) {
        buyer_id_num[i] = i + 1;
        pthread_create(&buyers[i], NULL, buyer, &buyer_id_num[i]);
    }

    // If all threads complete, join them. (Shouldn't be reached because of infinite loop "^-^ )
    pthread_join(provider_thread, NULL);
    for (int i = 0; i < num_buyers; i++) {
        pthread_join(buyers[i], NULL);
    }

    // Cleanup (Also shouldn't be reached.)
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&buffer_mutex);

    return 0;
}