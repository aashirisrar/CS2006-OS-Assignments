#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    string fileName;
    string substring;

    cout << "Enter the file to open:";
    cin >> fileName;
    cout << "Enter the substring to search for: ";
    cin >> substring;
    ifstream file(fileName);

    if (file)
    {
        string word;
        int countOfOccurences = 0;

        while (!file.eof())
        {
            file >> word;

            int ind = 0;

            int i = 0;
            while (ind < word.length())
            {
                if (word[ind] == substring[i])
                {
                    while (i < substring.length() && ind < word.length() && word[ind] == substring[i])
                    {
                        i++;
                        ind++;
                    }
                    if (i == substring.length())
                    {
                        countOfOccurences++;
                    }
                }
                else
                {
                    ind++;
                }
            }
        }

        cout << "The substring is found " << countOfOccurences << " times in the file\n";
        file.close();
    }
    else
    {
        cout << "Error opening the file\n";
    }
    return 0;
}