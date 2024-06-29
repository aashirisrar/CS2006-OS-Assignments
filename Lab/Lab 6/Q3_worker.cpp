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

    char arr[100];
    int arrLen;

    // Read array length
    int resp = read(fd, &arrLen, sizeof(int));

    // Acknowledge the sender
    write(fd, &resp, sizeof(int));
    sleep(1);  // 1 second delay

    // Read the string
    read(fd, arr, arrLen);

    int result = 0;
    char op = arr[0];

    int i=2;
    int num1;
    int num2;

    string temp;
    while (arr[i] != ' ') {
        temp += arr[i++];
    }

    num1 = stoi(temp);

    i++;
    temp = "";
    while (arr[i] != '\0') {
        temp += arr[i++];
    }
    num2 = stoi(temp);


    switch (op) {
        case '+': {
            result = num1 + num2;
            break;
        }
        case '-': {
            result = num1 - num2;
            break;
        }
        case '*': {
            result = num1 * num2;
            break;
        }
        case '/': {
            result = num1 / num2;
            break;
        }


    }

    // Send the modified string back
    write(fd, &result, arrLen);
    sleep(1);  // 1 second delay

    close(fd);

    return 0;
}
