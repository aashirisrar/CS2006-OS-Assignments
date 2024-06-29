#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void removesc(char *str) {
    int len = strlen(str);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if ((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void printArray(char* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c ", arr[i]);
    }
    printf("\n");
    printf("size: %d \n", size);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s abcd$$nice++\n", argv[0]);
        return 1;
    }

    char* arr = argv[1];
    int size = strlen(arr);
    int bytesize= sizeof(char) * strlen(arr);

    printf("Original Array:\n");
    printArray(arr, size);

    int pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        printf("Child Process 1\n");
        printf("PID: %d, Parent PID: %d\n", getpid(), getppid());
        printf("Number of Elements: %d\n", size);
        printf("Size in Bytes: %d\n", bytesize);
        removesc(arr);
        printf("Array after removing special characters: %s\n", arr);
        execl("./remove_special_chars", "remove_special_chars", arr, NULL);
        exit(0);
    } else {
        int pid2 = fork();
        if (pid2 < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            printf("\nChild Process 2\n PID: %d Parent PID: %d\n", getpid(), getppid());
            exit(0);
        } else {
            wait(NULL);
            printf("\nParent Process: Done\n");
            wait(NULL);
        }
    }

    return 0;
}
