#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "Usage: %s <file>\n", argv[0]); return 1; }

    int fd = open(argv[1], O_RDONLY);
    if (fd != -1) {

        // If the file exists

        printf("[PID %ld] File %s already exists\n", (long) getpid(), argv[1]);
        close(fd);
    } else if (errno != ENOENT) {
        perror("open");
        return 1;
    } else {

        // If the file does NOT exist

        printf("[PID %ld] File %s doesn't exist yet\n", (long) getpid(), argv[1]);
        sleep(5);
        fd = open(argv[1], O_WRONLY | O_CREAT, 0600);
        if (fd == -1) { perror("open"); return 1; }
        printf("[PID %ld] Created file %s (exclusively)\n", (long) getpid(), argv[1]);
        close(fd);
    }
    return 0;
}