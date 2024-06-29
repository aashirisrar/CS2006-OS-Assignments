#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>

using namespace std;
const int BUFFER_SIZE = 1024;

int main(int argc, char *argv[]) {
    char buff[BUFFER_SIZE];
    int fd[2];
    int f_read = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);
    int f_write = open(argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    // Create a pipe
    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }


    if (f_read == -1 || f_write == -1) {
        perror("Error opening files");
        return 1;
    }

    // Create a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        close(fd[0]);

        char c;

        ssize_t readBytes;
        while ((readBytes = read(f_read, buff, BUFFER_SIZE)) > 0) {
            write(fd[1], &readBytes, sizeof(int));
            write(fd[1], buff, readBytes);
            buff[readBytes] = '\0';
            cout<<"[PARENT:"<<getpid()<<"] - Size of Content to Send: "<<readBytes<<endl;
            cout<<"[PARENT:"<<getpid()<<"] - Content to Send: "<<buff;
        }

        close(f_read);
        close(fd[1]); // Close the write end of the pipe
        wait(NULL);

    } else {
        close(fd[1]);
        int readBytes;
        while (read(fd[0], &readBytes, sizeof(int)) > 0) {
            read(fd[0], buff, readBytes);
            // Ensure null termination
            buff[readBytes] = '\0';
            write(f_write, buff, readBytes);
            cout<<"[CHILD:"<<getpid()<<"] - Size of Content to Receive: "<<readBytes<<endl;
            cout<<"[CHILD:"<<getpid()<<"] - Content Received: "<<buff;
        }

        close(f_write);
        close(fd[0]); // Close the read end of the pipe
        exit(EXIT_SUCCESS);
    }


    return 0;
}