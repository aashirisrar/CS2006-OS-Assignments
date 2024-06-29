#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
    void *sharedMemory;
    char buffer[100];

    if (argc != 2) {
        cout << "error, e.g ./client file.txt";
        exit(1);
    }

    int fd_read = open(argv[1], O_RDONLY);
    if (!fd_read) {
        cout << "error opening file";
        exit(1);
    }

    read(fd_read, buffer, 100);
    cout<<"Sent data to server:";
    cout << buffer;

    int shmid = shmget((key_t) 1122, 1024, 0666 | IPC_CREAT);
    sharedMemory = shmat(shmid, NULL, 0);

    strcpy((char *) sharedMemory, buffer);
    sleep(10);

    int unlink_status = shmdt(sharedMemory);
    if(unlink_status<0)
    {
        cout<<"shmdt failed"<<endl;
        return 3;
    }

    return 0;
}

