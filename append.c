#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define oops(m, x) {perror(m); exit(x);}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: ./a.out cmd file");
        exit(1);
    }
    int pipefd[2];
    pid_t pid;
    char line[BUFSIZ];
    
    if (pipe(pipefd) == -1)
        oops("pipe", 2);

    if ((pid = fork()) == -1)
        oops("fork", 3)
    else if (pid > 0) {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
            oops("dup2", 4)
        close(pipefd[0]);
        FILE *fp;
        if ((fp = fopen(argv[2], "a")) == NULL )
            oops("open", 5);
        while (fgets(line, BUFSIZ, stdin) != NULL) {
            int len = strlen(line);
            if (fputs(line, fp) == EOF) 
                oops("fputs", 6);
        }
        fclose(fp);
        wait(NULL);
    } else {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            oops("dup2", 7);
        close(pipefd[1]);
        execlp(argv[1], argv[1], NULL);
        oops("execl", 8);
    }
}
