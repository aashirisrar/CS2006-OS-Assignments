#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;
#define PIPE_PERM (S_IRUSR | S_IWUSR)

int main() {
    int f_write = open("qq.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    char n[] = "Operating Systems";
    int len = 0;
    for (int i = 0; n[i] != '\0'; i++) len++;

    char dd;
    char t = 'X';
    if (f_write) {
        write(f_write, n, len);
        lseek(f_write, 0, SEEK_SET);  // Reset file pointer to the beginning

        while (read(f_write, &dd, 1)) {
            lseek(f_write, 1, SEEK_CUR);
            write(f_write, &t, 1);
        }

        lseek(f_write, 0, SEEK_SET);  // Reset file pointer to the beginning


        while (read(f_write, &dd, 1)) {
            lseek(f_write, 1, SEEK_CUR);
            write(1, &dd, 1);
        }
    }

    return 0;
}