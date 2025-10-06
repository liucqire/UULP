#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXARGS 20
#define ARGLEN 100

int main() {
    signal(SIGINT, SIG_IGN);

    char *arglist[MAXARGS + 1];
    int numargs;
    char argbuf[ARGLEN];
    char *makestring();
    void execute(char *arglist[]);

    numargs = 0;
    while (numargs < MAXARGS) {
        printf("Arg[%d]? ", numargs);
        if (fgets(argbuf, ARGLEN, stdin) == NULL) return 0;
        if (argbuf[0] != '\n') {
            if (strcmp("exit\n", argbuf) == 0) return 0;
            printf("%s", argbuf);
            argbuf[strlen(argbuf) - 1] = '\0';
            arglist[numargs++] = strdup(argbuf);
        } else {
            if (numargs > 0) {
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            } else {
                if (argbuf[0] == EOF)
                    return 0;
            }
        }
    }
}

void execute(char *arglist[]) {
    int pid, existstatus;
    pid = fork();
    switch(pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            execvp(arglist[0], arglist);
            perror("execvp failed");
            exit(1);
        default:
            waitpid(pid, &existstatus, 0);
            printf("child exited with status %d, %d\n",
                    existstatus>>8, existstatus&0377);
    }
}
