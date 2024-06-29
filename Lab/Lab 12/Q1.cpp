#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void replaceWordInFile(const char *fileName, const char *wordToReplace, const char *replacementWord) {
    int fd = open(fileName, O_RDWR);
    if (fd == -1) {
        std::cerr << "Error: Couldn't open the file\n";
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Error: Couldn't get file size\n";
        close(fd);
        return;
    }

    char *fileData = (char*)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fileData == MAP_FAILED) {
        std::cerr << "Error: Couldn't map the file to memory\n";
        close(fd);
        return;
    }

    close(fd);

    char *pos = strstr(fileData, wordToReplace);
    while (pos != nullptr) {
        // Replace the word
        memcpy(pos, replacementWord, strlen(replacementWord));
        pos = strstr(pos + strlen(replacementWord), wordToReplace);
    }

    // Unmap the file
    if (munmap(fileData, sb.st_size) == -1) {
        std::cerr << "Error: Couldn't unmap the file from memory\n";
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <file_name> <word_to_replace> <replacement_word>\n";
        return 1;
    }

    const char *fileName = argv[1];
    const char *wordToReplace = argv[2];
    const char *replacementWord = argv[3];

    replaceWordInFile(fileName, wordToReplace, replacementWord);

    return 0;
}
