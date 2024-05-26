#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    string userCommand;
    while (true) {
        // Prompting user for command
        cout << "Enter a command (cp, mkdir, ls), or type 'exit' to quit: ";
        getline(cin, userCommand);

        // Exit if the user inputs "exit"
        if (userCommand == "exit") {
            break;
        }

        // Tokenizing the command
        vector<string> tokens;
        string token;
        for (char c : userCommand) {
            if (c == ' ') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) {
            tokens.push_back(token);
        }

        // Converting tokens to char* array for execvp
        vector<char*> args(tokens.size() + 1);
        for (size_t i = 0; i < tokens.size(); ++i) {
            args[i] = &tokens[i][0];
        }
        args[tokens.size()] = nullptr;

        // Creating a child process
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Error: Fork failed." << endl;
            return 1;
        } else if (pid == 0) { // Child process
            // Executing the command
            execvp(args[0], args.data());
            cerr << "Error: Failed to execute command." << endl;
            return 1;
        } else { // Parent process
            // Waiting for the command to finish execution
            int status;
            waitpid(pid, &status, 0);
        }
    }

    cout << "Shell exited." << endl;
    return 0;
}
