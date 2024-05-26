#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm> 

using namespace std;

//split a string based on a delimiter
vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " students_list.txt section1.txt [section2.txt ...]" << endl;
        return 1;
    }

    vector<string> sectionFileNames;
    for (int i = 2; i < argc; ++i) {
        sectionFileNames.push_back(argv[i]);
    }

    // Open students file for writing the count and student data
    ofstream studentsFile(argv[1]);
    if (!studentsFile) {
        cerr << "Error: Unable to open students file." << endl;
        return 1;
    }

    // Write total number of students to students file
    int totalStudents = 0;
    for (const string& sectionFileName : sectionFileNames) {
        ifstream sectionFile(sectionFileName);
        if (sectionFile) {
            string line;
            while (getline(sectionFile, line)) {
                totalStudents++;
                studentsFile << line << endl; // Write student data to students_list.txt
            }
            sectionFile.close();
        }
    }
    studentsFile.seekp(0); // Move back to the beginning of the file
    studentsFile << totalStudents << endl; // Update total student count
    studentsFile.close();

    return 0;
}
