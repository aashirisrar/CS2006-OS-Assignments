#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

const int ARRLENGTH = 5;

int main() {
    int fd[2];

    if (pipe(fd) == -1) cout << "Error!" << endl;

    int st[] = {1, 2, 3, 4, 5};
    int len = sizeof(int) * ARRLENGTH;

    pid_t pid = fork();
    if (pid > 0) {
        close(fd[0]);
        write(fd[1], st, len);
    } else {
        int arr[len];
        close(fd[1]);
        read(fd[0], arr, len);

        int sum = 0;
        for (int i = 0; i < ARRLENGTH; i++) sum += arr[i];

        cout<<sum<<endl;
    }

    return 0;
}