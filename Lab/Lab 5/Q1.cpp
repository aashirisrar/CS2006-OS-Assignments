#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

int main() {
    int fd[2];

    if (pipe(fd) == -1) cout << "Error!" << endl;

    char st[] = "Hi There";
    int len = 0;
    for (int i = 0; st[i] != '\0'; ++i) {
        len++;
    }

    pid_t pid = fork();
    if (pid > 0) {
        close(fd[0]);
        write(fd[1], st, len);
    } else {
        char arr[len];
        close(fd[1]);
        read(fd[0], arr, len);
        for (int i = 0; i < len; i++) // Adjust loop limit to avoid modifying null terminator
        {
            if (isalpha(arr[i])) {
                if (isupper(arr[i]))
                    arr[i] += 32;
                else
                    arr[i] -= 32;
            }
        }
        write(1, arr, len);
    }

    return 0;
}