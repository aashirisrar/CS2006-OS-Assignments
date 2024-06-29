#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

using namespace std;

int i;
int laps;
int time1;

void printTotalTime() {
    cout << "Total time in seconds: " << time1 * laps;
}

void handler(int sig_num) {
    if (sig_num == SIGALRM) {
        cout << "Lap: " << i << " Completed\n";
    }
    if (sig_num == SIGCHLD) {
        cout << "Received SIGCHLD" << endl;
        printTotalTime();
    }
}


int main() {
    signal(SIGALRM, handler);
    signal(SIGCHLD, handler);
    cout << "Enter the number of laps:";
    cin >> laps;

    cout << "Enter the time of each lap:";
    cin >> time1;

    pid_t pid = fork();

    if (pid) {
        wait(NULL);
    } else {
        for (i = 1; i <= laps; i++) {
            alarm(time1);
            sleep(time1);
        }

    }

    return 0;
}

