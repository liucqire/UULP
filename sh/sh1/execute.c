#include "smsh.h"
#include <stdio.h>

int execute(char **arglist) {
    int pid;
    int child_info = -1;
    if (arglist[0] == NULL)
        return 0;
    if ((pid = fork()) == -1)
        perror("fork");
    else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(arglist[0], arglist);
        perror("cannot execvp command");
        exit(1);
    } else {
        if (waitpid(pid, &child_info, 0) == -1)
            perror("wait");
    }
    return child_info;
}
