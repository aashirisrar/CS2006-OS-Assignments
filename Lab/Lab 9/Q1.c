#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 10
#define NUM_PER_THREAD 10
#define TOTAL_NUMS THREAD_COUNT * NUM_PER_THREAD

int num_list[TOTAL_NUMS]; // Global list to hold the numbers
pthread_mutex_t mutex; // Mutex to protect the list

// Function for each thread to insert numbers into the list
void *number_insert(void *arg) {
    int thread_num = *(int *)arg;
    for (int i = thread_num - 1; i < TOTAL_NUMS; i += THREAD_COUNT) {
        pthread_mutex_lock(&mutex); // Lock before accessing shared data
        num_list[i] = thread_num;
        pthread_mutex_unlock(&mutex); // Unlock after modifying shared data
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int thread_nums[THREAD_COUNT];

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < THREAD_COUNT; ++i) {
        thread_nums[i] = i + 1;
        pthread_create(&threads[i], NULL, number_insert, &thread_nums[i]);
    }

    // Join threads
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print the resulting list
    printf("List: [");
    for (int i = 0; i < TOTAL_NUMS; ++i) {
        printf("%d", num_list[i]);
        if (i < TOTAL_NUMS - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    // Calculate sum
    int total_sum = 0;
    for (int i = 0; i < TOTAL_NUMS; ++i) {
        total_sum += num_list[i];
    }

    // Print total sum
    printf("Total Sum: %d\n", total_sum);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
