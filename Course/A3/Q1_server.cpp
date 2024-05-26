#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <cstring>
#include <semaphore.h>

using namespace std;

sem_t *semaphore1;
sem_t *semaphore2;
pthread_mutex_t lock;

int main() {
    while (true) {
        cout << "Waiting for client...\n";
        char c;
        string s;

        void *sharedMemory;
        char buffer[100];
        int shmid = shmget((key_t) 1122, 1024, 0666 | IPC_CREAT);
        sharedMemory = shmat(shmid, NULL, 0);

        int id1 = shmget(1, 1024, IPC_CREAT | 0666);
        int id2 = shmget(2, 1024, IPC_CREAT | 0666);

        semaphore1 = (sem_t *) shmat(id1, NULL, 0);
        sem_init(semaphore1, 1, 0);

        semaphore2 = (sem_t *) shmat(id2, NULL, 0);
        sem_init(semaphore2, 2, 0);

        sem_wait(semaphore1);

        cout << "Client recieved!\n";
        strcpy(buffer, "--------------Welcome to Movie World--------------\n");
        strcpy((char *) sharedMemory, buffer);
        sem_post(semaphore2);

        sem_wait(semaphore1);
        int fd_movie_read = open("movieTicketsInfo.txt", O_RDONLY);
        // Read movie tickets info file
        read(fd_movie_read, buffer, 100);
        // send data to client via pipe
        strcpy((char *) sharedMemory, buffer);

        sem_post(semaphore2);
        sem_wait(semaphore1);
        cout << "Extracting movie and tickets numbers\n";
        int movieNum = 0;
        int numOfTickets = 0;
        string n;
        string m;
        strcpy(buffer, (char *) sharedMemory);

        int i = 0;
        int p = 0;
        while (buffer[p] != '\n') {
            // read movie number and number of tickets from the client
            if (i == 0) {
                m += buffer[p];
            }
            if (i == 1) {
                n += buffer[p];
            }
            if (buffer[p] == ' ') {
                i++;
            }
            p++;
        }
        numOfTickets = stoi(n);
        movieNum = stoi(m);

        cout << "Movie number asked: " << movieNum << endl;
        cout << "Number of tickets asked: " << numOfTickets << endl;

        // also check if tickets are available
        close(fd_movie_read);
        fd_movie_read = open("movieTicketsInfo.txt", O_RDONLY);
        i = 0;
        int pass = 0;
        int success = 0;

        int ticketsAvailable = 0;
        string movieName;

        string num;
        while (read(fd_movie_read, &c, sizeof(char)) > 0) {
            if (i == movieNum) {
                if (pass == 1) {
                    if (c == '/') {

                    } else {
                        movieName += c;
                    }
                }
                if (pass == 2) {
                    num += c;
                    if (c == '\n') {
                        ticketsAvailable = stoi(num);
                        success = 1;
                        break;
                    }
                }
                if (c == '/') pass++;
            }
            if (c == '\n') {
                i++;
            }
        }
        if (success) {
            cout << "Tickets available:" << ticketsAvailable << endl;

            if (ticketsAvailable >= numOfTickets) {
                int totalBill = numOfTickets * 500;
                cout<<"Total Bill:"<<totalBill<<endl;
                string total = to_string(totalBill);
                string temp = "Your total bill for " + movieName + " is " + total + " Rs" + "\n" +
                              "Press y to proceed or press c to cancel" + "\n";
                strcpy(buffer, temp.c_str());

                strcpy((char *) sharedMemory, buffer);

                sem_post(semaphore2);
                sem_wait(semaphore1);

                strcpy(buffer, (char *) sharedMemory);
                if (buffer[0] == 'y') {
                    cout << "Received y from client\n";
                    // reserve the ticket
                    int fd_movie_write = open("movieTicketsInfo.txt", O_WRONLY);
                    int remainingTickets = ticketsAvailable - numOfTickets;
                    string remTickets = to_string(remainingTickets);
                    remTickets += '\n';
                    cout << "Remaining tickets after reservation:" << remTickets;
                    // decrease the number of movie tickets in the movie tickets info file
                    pthread_mutex_init(&lock, NULL);
                    pthread_mutex_lock(&lock);
                    cout<<"Decreasing tickets\n";
                    i = 0;
                    close(fd_movie_read);
                    int currCharPos = 0;
                    fd_movie_read = open("movieTicketsInfo.txt", O_RDONLY);
                    pass = 0;
                    while (read(fd_movie_read, &c, sizeof(char)) > 0) {
                        if (i == movieNum) {
                            if (pass == 2) {
                                lseek(fd_movie_write, currCharPos, SEEK_SET);
                                write(fd_movie_write, remTickets.c_str(), remTickets.length());
                                break;
                            }
                            if (c == '/') pass++;
                        }

                        if (c == '\n') i++;

                        currCharPos++;
                    }
                    pthread_mutex_unlock(&lock);

                    cout << "Asking user for cnic" << endl;
                    s = "Enter your 4 digit cnic:\n";
                    strcpy((char *) sharedMemory, s.c_str());
                    sem_post(semaphore2);
                    sem_wait(semaphore1);
                    strcpy(buffer, (char*)sharedMemory);
                    string cnic;
                    for(int j=0; j<4;j++)
                    {
                        cnic+=buffer[j];
                    }
                    cout<<"CNIC recieved from client:"<<cnic<<endl;

                    cout<<"Saving booking record"<<endl;
                    success = 0;
                    int booking_fd_read = open("BookingRecord.txt", O_RDWR | O_CREAT, 0666);
                    int booking_fd_write = open("BookingRecord.txt", O_WRONLY);

                    string toWrite = cnic + '/' + to_string(movieNum) + '/' + to_string(numOfTickets) + '\n';

                    // update the number at top of file first
                    s = "0";
                    while (read(booking_fd_read, &c, sizeof(char)) > 0) {
                        s += c;
                    }
                    int currRecordsInFile = stoi(s);
                    currRecordsInFile++;
                    string t = to_string(currRecordsInFile);
                    t += '\n';
                    lseek(booking_fd_write, 0, SEEK_SET);
                    write(booking_fd_write, t.c_str(), t.length());

                    // write the current record at the end of file
                    lseek(booking_fd_write, 0, SEEK_END);
                    write(booking_fd_write, toWrite.c_str(), toWrite.length());

                    close(booking_fd_write);
                    close(booking_fd_read);
                    success = 1;

                    if (success) {
                        cout << "Reservation Done!\n" << endl;
                        strcpy((char *) sharedMemory, "Reservation Done!\n");
                        sem_post(semaphore2);
                    } else {
                        cout << "Error!\n" << endl;
                        strcpy((char *) sharedMemory, "Reservation Cancelled!\n");
                    }
                }
            } else {
                cout << "Not enough tickets" << endl;
                strcpy((char *) sharedMemory, "Not enough tickets\n");
            }
        } else {
            cout << "Error! Movie not available!" << endl;
            strcpy((char *) sharedMemory, "Error! Movie not available!");
        }

    }
}
