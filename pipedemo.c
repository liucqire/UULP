#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int len, i, apipe[2];
    char buf[BUFSIZ];

    if (pipe(apipe) == -1) {
        perror("could not make pipe");
        exit(1);
    }
    printf("Got a pipe! It is file descriptors: {%d %d}\n", apipe[0], apipe[1]);
    while (fgets(buf, BUFSIZ, stdin) != NULL) {
        len = strlen(buf);
        if (write(apipe[1], buf, len) != len) {
            perror("write error");
            exit(1);
        } 
        for (int i = 0; i < len; i++)
            buf[i] = 'X';
        len = read(apipe[0], buf, BUFSIZ);
        if (len == -1) {
            perror("reading from pipe");
            break;
        }
        if (write(STDOUT_FILENO, buf, len) != len) {
            perror("writing to stdout");
            break;
        }
    }
}
