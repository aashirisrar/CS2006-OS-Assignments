#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <pthread.h>

using namespace std;

struct P {
    string pipeName;
    string file;
};

void *worker(void *param) {
    P *p = (P *) param;

    int fd_read = open(p->file.c_str(), O_RDONLY);
    int fd_write = open(p->pipeName.c_str(), O_WRONLY);

//    cout << "In worker of file " << p->file.c_str() << endl;
    char b;
    string s;
    while (read(fd_read, &b, sizeof(char)) > 0) {
        // send response back to client
        write(fd_write, &b, sizeof(char));
        s += b;
    }
    cout << "Contents read from file :\n" << s << "\n";

    cout << "Sending file contents to client\n";

    close(fd_write);
    close(fd_read);

    delete p;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    while (true) {
        cout << "Waiting for client...\n";

        char *pipe_name = argv[1];
        int fd = open(pipe_name, O_RDONLY);

        char c;
        string s;
        vector<string> st;


        int i = 0;
        // Read file names
        while (read(fd, &c, sizeof(char)) > 0) {
            if (c == ' ') {
                st.push_back(s);
                cout << "Received file name: " << s << "\n";
                s = "";
                continue;
            }
            if (c == '\0') {
                st.push_back(s);
                cout << "Received file name: " << s << "\n";
                s = "";
                break;
            }
            s += c;
        }

        pthread_t threadID;
        for (string &fileName: st) {
            // Allocate memory for each thread's P struct
            P *pp = new P;
            pp->pipeName = pipe_name;
            pp->file = fileName;

            cout << "Thread created with ID: " << pthread_self() << "\n";
            pthread_create(&threadID, NULL, worker, (void *) pp);
        }

        // Wait for all threads to finish
        pthread_join(threadID, NULL);

        // Close the pipe
        close(fd);

    }
}
