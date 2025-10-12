#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define oops(m) {perror(m);exit(-1);}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage ./a.out cmd file");
        exit(-1);
    }
    pid_t pid;
    int pipefd[2];
    if (pipe(pipefd) == -1)
        oops("pipe");

    if ((pid = fork()) == -1)
        oops("fork")
    else if (pid > 0) {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            oops("dup2");
        close(pipefd[1]);
        FILE *fp;
        char line[BUFSIZ];
        if ((fp = fopen(argv[2], "r")) == NULL)
            oops("fopen");
        while (fgets(line, BUFSIZ, fp) != NULL) {
            int len = strlen(line);
            if (write(STDOUT_FILENO, line, len) != len)
                oops("write");
        }
        fclose(fp);
        // two crucial lines, or leading to deadlock
        close(STDOUT_FILENO);
        wait(NULL);
    } else if (pid == 0) {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
            oops("dup2");
        close(pipefd[0]);
        execlp(argv[1], argv[1], NULL);
        oops("execlp");
    }
}
