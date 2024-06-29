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

//    char st[] = "Cats";
//    int stringLen = strlen(st) + 1; // Include the null terminator

    int arr[] = {1,2,3,4,5};
    int sizeArr = sizeof(int) * 5;

    // Write string length
    write(fd, &sizeArr, sizeof(int));
    sleep(1);  // 1 second delay

    // Wait for acknowledgment from the receiver
    int resp;
    read(fd, &resp, sizeof(int));

    if (resp > 0) {
        // Write the array
        write(fd, arr, sizeArr);
        sleep(1);  // 1 second delay

        int res;
        read(fd, &res, sizeof(int));
        cout << "Received Array Sum: " << res << endl;
    } else {
        cout << "Error! while reading" << endl;
    }

    close(fd);

    return 0;
}
