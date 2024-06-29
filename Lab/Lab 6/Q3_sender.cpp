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

    char st[] = "- 41 1";
    int stringLen = strlen(st) + 1; // Include the null terminator

    // Write string length
    write(fd, &stringLen, sizeof(int));
    sleep(1);  // 1 second delay

    // Wait for acknowledgment from the receiver
    int resp;
    read(fd, &resp, sizeof(int));

    if (resp) {
        // Write the array
        write(fd, st, stringLen);
        sleep(1);  // 1 second delay

        int res;
        read(fd, &res, sizeof(int));
        cout << "Received Result: " << res << endl;
    } else {
        cout << "Error! while reading" << endl;
    }

    close(fd);

    return 0;
}
