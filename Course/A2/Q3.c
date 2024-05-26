#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Function executed by each thread
void *thread_function(void *arg) {
    int thread_id = *((int *) arg);
    printf("Hello from thread %lu - I was created in iteration %d\n", pthread_self(), thread_id - 1);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    int *thread_ids = malloc(num_threads * sizeof(int));

    if (threads == NULL || thread_ids == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i + 1;
        if (i % 4 == 0 || i % 5 == 0) {
            sleep(1); // Sleep for 1 second for every 4 or 5 threads created
        }
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("Thread creation failed");
            return 1;
        }
        printf("I am thread %lu. Created new thread (%lu) in iteration %d\n", pthread_self(), threads[i], i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_ids);

    return 0;
}
