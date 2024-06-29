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

    int arr[100];
    int arrLen;

    // Read array length
    int resp = read(fd, &arrLen, sizeof(int));

    // Acknowledge the sender
    write(fd, &resp, sizeof(int));
    sleep(1);  // 1 second delay

    // Read the string
    read(fd, arr, arrLen);

    int sum = 0;
    for (int i = 0; i < arrLen - 1; i++) // Adjust loop limit to avoid modifying null terminator
    {
        sum += arr[i];
    }

    // Send the modified string back
    write(fd, &sum, arrLen);
    sleep(1);  // 1 second delay

    close(fd);

    return 0;
}
