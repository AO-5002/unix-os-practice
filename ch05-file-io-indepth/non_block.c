//
// Created by Andres Ortiz Osorio on 10/7/26.
//
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int flags = fcntl(0, F_GETFL), count = 0;
    char buf[1024];
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        printf("%d\n", count++);
        ssize_t n = read(0, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("User typed: %s", buf);
            break;
        }

        sleep(1);
    }

    fcntl(0, F_SETFL, flags);
}
