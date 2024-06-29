#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *fib_series;
    int size;
} FibSeries;

void *fibonacciGenerator(void *param) {
    int n = *((int *) param);
    int *fib_series = (int *) malloc((n + 1) * sizeof(int));
    if (fib_series == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        pthread_exit(NULL);
    }

    int first = 0, second = 1;
    fib_series[0] = first;
    fib_series[1] = second;
    for (int i = 2; i < n; i++) {
        fib_series[i] = fib_series[i - 1] + fib_series[i - 2];
    }

    FibSeries *fib_series_struct = (FibSeries *) malloc(sizeof(FibSeries));
    if (fib_series_struct == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(fib_series);
        pthread_exit(NULL);
    }

    fib_series_struct->fib_series = fib_series;
    fib_series_struct->size = n;
    pthread_exit((void *) fib_series_struct);
}

void *countEvenNumbers(void *param) {
    FibSeries *fib_series_struct = (FibSeries *) param;
    int count = 0;
    for (int i = 0; i < fib_series_struct->size; i++) {
        if (fib_series_struct->fib_series[i] % 2 == 0 && fib_series_struct->fib_series[i] != 0) {
            count++;
        }
    }
    pthread_exit((void *) (intptr_t) count);
}

void *countOddNumbers(void *param) {
    FibSeries *fib_series_struct = (FibSeries *) param;
    int count = 0;
    for (int i = 0; i < fib_series_struct->size; i++) {
        if (fib_series_struct->fib_series[i] % 2 != 0) {
            count++;
        }
    }
    pthread_exit((void *) (intptr_t) count);
}

void *calculateSum(void *param) {
    FibSeries *fib_series_struct = (FibSeries *) param;
    long long sum = 0;
    for (int i = 0; i < fib_series_struct->size; i++) {
        sum += fib_series_struct->fib_series[i];
    }

    FILE *file = fopen("sum.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        pthread_exit(NULL);
    }
    fprintf(file, "%lld", sum);
    fclose(file);

    pthread_exit((void *) (intptr_t) sum);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    pthread_t threadID[4];
    void *thread_return;

    // Fibonacci generator thread
    pthread_create(&threadID[0], NULL, fibonacciGenerator, (void *) &n);
    pthread_join(threadID[0], &thread_return);
    FibSeries *fib_series_struct = (FibSeries *) thread_return;
    printf("ID: %lu, ", threadID[0]);
    printf("Series: ");
    for (int i = 0; i < fib_series_struct->size; i++) {
        printf("%d ", fib_series_struct->fib_series[i]);
    }
    printf("\n");

    // Count even numbers thread
    pthread_create(&threadID[1], NULL, countEvenNumbers, (void *) fib_series_struct);
    pthread_join(threadID[1], &thread_return);
    int even_count = (int) (intptr_t) thread_return;
    printf("ID: %lu, Even Numbers: %d\n", threadID[1], even_count);

    // Count odd numbers thread
    pthread_create(&threadID[2], NULL, countOddNumbers, (void *) fib_series_struct);
    pthread_join(threadID[2], &thread_return);
    int odd_count = (int) (intptr_t) thread_return;
    printf("ID: %lu, Odd Numbers: %d\n", threadID[2], odd_count);

    // Calculate sum thread
    pthread_create(&threadID[3], NULL, calculateSum, (void *) fib_series_struct);
    pthread_join(threadID[3], &thread_return);
    long long sum = (long long) (intptr_t) thread_return;
    printf("ID: %lu, Sum: %lld\n", threadID[3], sum);

    // Free allocated memory
    free(fib_series_struct->fib_series);
    free(fib_series_struct);

    return 0;
}
