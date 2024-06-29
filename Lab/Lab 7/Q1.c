#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker(void *param) {
    printf("Process ID: %d\n",getpid());
    printf("Thread ID: %lu\n",pthread_self());
    pthread_exit(0);
}

int main() {
    int n = 5;
    pthread_t th[5];
    for (int i = 0; i < n; ++i) {
        pthread_create(th + i, NULL, worker, NULL);
//        printf("Thread %d has started\n", i);
    }
    for (int i = 0; i < n; ++i) {
        pthread_join(th[i], NULL);
//        printf("Thread %d has finished execution\n", i);
    }

    return 0;
}






