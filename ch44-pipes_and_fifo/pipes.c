//
// Created by Andres Ortiz Osorio on 10/18/26.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);
    int pid = fork();

    // Child Process
    if (pid == 0) {
        // No writing
        close(fd[1]);
        char buffer[100];
        ssize_t n = read(fd[0], buffer, sizeof(buffer) - 1);
        if (n < 0) perror("read");
        buffer[n] = '\0';
        printf("Message from parent: %s\n", buffer);
        close(fd[0]);
        return 0;
    }

    // Parent Process
    // No reading

    close(fd[0]);
    const char *message = "Hello World";
    write(fd[1], message, strlen(message));
    close(fd[1]);
    wait(NULL);
    return 0;
}


