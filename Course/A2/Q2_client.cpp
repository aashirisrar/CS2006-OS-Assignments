#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " pipe_name filename1..." << endl;
        return 1;
    }

    char *pipe_name = argv[1];
    int fd = open(pipe_name, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    string st;
    for (int i = 2; i < argc; i++) {
        st += argv[i];
        if (i != argc - 1) st += ' ';
        if(i== argc -1) st+='\0';
    }

    cout<<"Got filename "<< st << " from command line\n";

    cout<<"Sending filename to server\n";

    // Write string
    ssize_t bytes_written = write(fd, st.c_str(), st.length());
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        return 1;
    }

    cout<<"Waiting for server to respond...\n";

    // Close the write end of the pipe
    close(fd);

    // Open the pipe for reading
    fd = open(pipe_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Read response from server
    char buffer[BUFSIZ];
    ssize_t bytes_read;
    cout<<"Contents of file from pipe:\n";

    while ((bytes_read = read(fd, buffer, BUFSIZ)) > 0) {
        // Write response to stdout
        write(1, buffer, bytes_read);
    }


    if (bytes_read == -1) {
        perror("read");
    }

    // Close the pipe
    close(fd);

    return 0;
}
