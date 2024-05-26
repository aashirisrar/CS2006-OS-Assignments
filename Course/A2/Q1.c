#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

typedef struct {
    int *arr;
    int size;
} arrayStruct;

void *calculateAverage(void *param) {
    arrayStruct *arr = (arrayStruct *) param;

    int sum = 0;
    for (int i = 0; i < arr->size; i++) {
        sum += arr->arr[i];
    }

    float avg = sum/arr->size;

    pthread_exit((void *) (intptr_t) avg);
}

void *maxValue(void *param) {
    arrayStruct *arr = (arrayStruct *) param;

    int max = -INT_MAX;
    for (int i = 0; i < arr->size; i++) {
        if (arr->arr[i] > max) max = arr->arr[i];
    }

    pthread_exit((void *) (intptr_t) max);
}

void *minValue(void *param) {
    arrayStruct *arr = (arrayStruct *) param;

    int min = INT_MAX;
    for (int i = 0; i < arr->size; i++) {
        if (arr->arr[i] < min) min = arr->arr[i];
    }

    pthread_exit((void *) (intptr_t) min);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number1> <number2> ... <numberN>\n", argv[0]);
        return 1; 
    }


    int size = argc - 1;


    int *array = (int *) malloc(size * sizeof(int));


    if (array == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1; 
	}

    for (int i = 0; i < size; i++) {
        array[i] = atoi(argv[i + 1]);
    }

    arrayStruct *arr = (arrayStruct *) malloc(sizeof(arrayStruct));
    arr->size = size;
    arr->arr = array;

    pthread_t threadID[3];
    void *thread_return;

    // Calculate sum thread
    pthread_create(&threadID[0], NULL, calculateAverage, (void *) arr);
    pthread_join(threadID[0], &thread_return);
    long long sum = (long long) (intptr_t) thread_return;
    printf("ID: %lu, Average: %lld\n", threadID[0], sum);

    // Calculate max thread
    pthread_create(&threadID[1], NULL, maxValue, (void *) arr);
    pthread_join(threadID[1], &thread_return);
    long long max = (long long) (intptr_t) thread_return;
    printf("ID: %lu, Max: %lld\n", threadID[1], max);

    // Calculate min thread
    pthread_create(&threadID[2], NULL, minValue, (void *) arr);
    pthread_join(threadID[2], &thread_return);
    long long min = (long long) (intptr_t) thread_return;
    printf("ID: %lu, Min: %lld\n", threadID[2], min);

    // Free allocated memory
    free(arr->arr);
    free(arr);

    return 0;
}
