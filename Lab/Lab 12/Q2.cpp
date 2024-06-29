#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define FILE_SIZE 100

void* replaceIntegers(void* arg) {
    int* mapPtr = (int*)arg;
    for (int i = 0; i < FILE_SIZE / 2; ++i) {
        mapPtr[i] = ' '; // Replace integers with spaces
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_name>\n";
        return 1;
    }

    const char *fileName = argv[1];
    int fd = open(fileName, O_RDWR);
    if (fd == -1) {
        std::cerr << "Error: Couldn't open the file\n";
        return 1;
    }

    // Memory map the file
    int* fileData = (int*)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fileData == MAP_FAILED) {
        std::cerr << "Error: Couldn't map the file to memory\n";
        close(fd);
        return 1;
    }

    close(fd);

    pthread_t thread1, thread2;

    // Create the first thread to replace integers in the first half of the map
    if (pthread_create(&thread1, nullptr, replaceIntegers, (void*)fileData) != 0) {
        std::cerr << "Error: Couldn't create thread 1\n";
        munmap(fileData, FILE_SIZE);
        return 1;
    }

    // Create the second thread to replace integers in the second half of the map
    if (pthread_create(&thread2, nullptr, replaceIntegers, (void*)(fileData + FILE_SIZE / 2)) != 0) {
        std::cerr << "Error: Couldn't create thread 2\n";
        munmap(fileData, FILE_SIZE);
        return 1;
    }

    // Wait for both threads to finish
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    // Unmap the file
    if (munmap(fileData, FILE_SIZE) == -1) {
        std::cerr << "Error: Couldn't unmap the file from memory\n";
        return 1;
    }

    return 0;
}
