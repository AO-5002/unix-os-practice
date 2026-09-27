//
// Created by Andres Ortiz Osorio on 9/26/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef enum { COPY, OTHER } CMD;
int encode(const char *operation);
void copy_then_output(int inputFd, int outputFd);
void close_fds(const int *arr_fds, int size);

int main(int argc, char *argv[]) {
    chdir("ch04-file-io");
    if (argc != 4) { printf("Error: Usage: %s <input file> <output file>\n", argv[0]); }

    // Open the input file & write to the output file

    const mode_t mode = 0600;
    int inputFd = open(argv[2], O_RDONLY);
    if (inputFd == -1) { printf("Error: %s\n", strerror(errno)); return 1; }

    int outputFd = open(argv[3], O_EXCL | O_WRONLY | O_CREAT, mode);
    if (outputFd == -1) { printf("Error: %s\n", strerror(errno)); return 1; }

    switch (encode(argv[1])) {
        case COPY: copy_then_output(inputFd, outputFd);
        case OTHER: { close_fds((int[]){inputFd, outputFd}, 2); return 0; }
        default: { close_fds((int[]){inputFd, outputFd}, 2); return 1; }
    }
}

void close_fds(const int *arr_fds, const int size) {
    for (int i = 0; i < size; i++) { close(arr_fds[i]); }
}

void copy_then_output(const int inputFd, const int outputFd) {
    char buffer[BUFFER_SIZE];

    // Read until read returns 0 (which indicates the end of the file)
    ssize_t nums_read;
    while ((nums_read = read(inputFd, buffer, sizeof(buffer))) > 0) write(outputFd, buffer, nums_read);
    if (nums_read == -1) perror("read");
}

int encode(const char *operation) {
    if (strcmp(operation, "copy") == 0) { return COPY; }
    return OTHER;
}

