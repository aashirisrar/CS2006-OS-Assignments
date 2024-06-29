#include <bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
#define PIPE_PERM (S_IRUSR | S_IWUSR)

int main(int argc, char *argv[])
{
    char *pipe_name = argv[1];
    int status = mkfifo(pipe_name, PIPE_PERM);
    cout<<status<<endl;

    return 0;
}