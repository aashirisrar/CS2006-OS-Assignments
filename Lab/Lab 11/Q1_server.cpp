#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <cstring>
#include <semaphore.h>

using namespace std;

int main() {
    char c;
    string s;

    void *sharedMemory;
    char buffer[100];
    int shmid = shmget((key_t) 1122, 1024, 0666 | IPC_CREAT);
    sharedMemory = shmat(shmid, NULL, 0);

    strcpy(buffer, (char *) sharedMemory);
    cout << "Server recieved:"<<buffer;
    int num[100]={0};
    int k=0;
    int i=0;
    for(; buffer[i]!='\0';i++)
    {
        s+=buffer[i];
        if(buffer[i]==' ')
        {
            num[k++] = stoi(s);
            s="";
        }
    }
    num[k++] = stoi(s);

    int sum=0;
    float avg=0;
    for(int j=0; j<k;j++)
    {
        sum+=num[j];
    }
    avg = sum/k;

    cout<<"sum:"<<sum<<endl;
    cout<<"average:"<<avg<<endl;

    int unlink_status = shmdt(sharedMemory);
    if(unlink_status<0)
    {
        cout<<"shmdt failed"<<endl;
        return 3;
    }

    return 0;
}