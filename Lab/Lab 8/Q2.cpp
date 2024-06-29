#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main()
{
    int inputfd = open("input.txt", O_RDONLY, 0666);
    int outputfd = open("output.txt", O_WRONLY | O_CREAT, 0666);

    int originalInput = dup(0);
    dup2(inputfd, 0);

    int originalOutput = dup(1);
    dup2(outputfd, 1);

    string s;
    char c;
    while (read(0, &c, sizeof(char)))
    {
        s += c;
    }
    s += '\0';
    // Print to file
    write(1, s.c_str(), strlen(s.c_str()));

    dup2(originalInput, 0);  // Restore stdin
    dup2(originalOutput, 1); // Restore stdout

    // Print to console
    // cout << "\nPrinting to console now..." << endl;

    close(inputfd);
    close(outputfd);

    return 0;
}
