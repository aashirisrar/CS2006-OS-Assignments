#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *summation(void *param) {
    int val = *((int *)param); // Cast and dereference param to get the value of val

    int *sum_ptr = (int *)malloc(sizeof(int)); // Allocate memory for the sum
    if (sum_ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        pthread_exit(NULL);
    }

    int sum = 0;
    for(int i = 0; i <= val; i++) {
        sum += i;
    }

    *sum_ptr = sum; // Store the sum in the allocated memory

    pthread_exit(sum_ptr); // Return the pointer to the sum
}

int main() {
    int val = 5;
    pthread_t threadID;
    pthread_create(&threadID, NULL, summation, (void *)&val);

    void *result;
    pthread_join(threadID, &result);

    int *sum_ptr = (int *)result;
    if (sum_ptr != NULL) {
        printf("After Thread Execution Summation = %d\n", *sum_ptr);
        free(sum_ptr); // Free allocated memory
    }

    return 0;
}
