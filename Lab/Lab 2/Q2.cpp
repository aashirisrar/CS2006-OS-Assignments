#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main()
{
    string fileName;
    string substring;

    cout << "Enter the file to open:";
    cin >> fileName;

    ifstream file(fileName);

    if (fork())
    {
        wait(NULL);
        cout << "Program completed.\n";
    }
    else
    {
        if (file)
        {
            char ch;
            int i = 0;
            while (file.get(ch))
            {
                if (!isalnum(ch) && ch != ' ' && ch != '\n')
                {
                    cout << "Special symbol found: " << ch << " at postition " << i << "\n";
                }
                i++;
            }
            file.close();
        }
        else
        {
            cout << "Error opening the file\n";
        }
    }
    return 0;
}