#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <semaphore.h>

using namespace std;

sem_t *semaphore1;
sem_t *semaphore2;

int main() {
    void *sharedMemory;
    char buffer[100];

    int shmid = shmget((key_t) 1122, 1024, 0666);
    sharedMemory = shmat(shmid, NULL, 0);

    int id1 = shmget(1, 1024, 0666);
    semaphore1 = (sem_t *) shmat(id1, NULL, 0);

    int id2 = shmget(2, 1024, 0666);
    semaphore2 = (sem_t *) shmat(id2, NULL, 0);

    sem_post(semaphore1);
    sem_wait(semaphore2);
    cout << (char *) sharedMemory;

    sem_post(semaphore1);
    sem_wait(semaphore2);
    cout << "In Theatres:\n" << (char *) sharedMemory;

    cout<<"Enter Movie and Number of Tickets:\n";
    fgets(buffer, sizeof(buffer), stdin);
    strcpy((char*)sharedMemory, buffer);
    sem_post(semaphore1);
    sem_wait(semaphore2);

    cout<<(char*)sharedMemory;
    // yes or no
    fgets(buffer, sizeof(buffer), stdin);
    strcpy((char*)sharedMemory, buffer);

    sem_post(semaphore1);
    sem_wait(semaphore2);

    cout<<(char*)sharedMemory;
    // cnic
    fgets(buffer, sizeof(buffer), stdin);
    strcpy((char*)sharedMemory, buffer);

    sem_post(semaphore1);
    sem_wait(semaphore2);

    cout<<(char*)sharedMemory;

    return 0;
}

