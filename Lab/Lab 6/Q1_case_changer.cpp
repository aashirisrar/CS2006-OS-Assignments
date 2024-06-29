#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
    char *pipe_name = argv[1];
    int fd = open(pipe_name, O_RDWR);

    char st[100];
    int strLen;

    // Read string length
    int resp = read(fd, &strLen, sizeof(int));

    // Acknowledge the sender
    write(fd, &resp, sizeof(int));
    sleep(1);  // 1 second delay

    // Read the string
    read(fd, st, strLen);

    for(int i=0; i<strLen-1; i++) // Adjust loop limit to avoid modifying null terminator
    {
        if(isupper(st[i]))
            st[i] += 32;
        else
            st[i] -= 32;
    }

    cout<<st<<"<-Child"<<endl;
    // Send the modified string back
    write(fd, st, strLen);
    sleep(1);  // 1 second delay

    close(fd);

    return 0;
}
