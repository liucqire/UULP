#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define NARG 100
#define LENARG 20

int main() {
    char *arglist[NARG];
    char buf[LENARG];
    int i; 
    for (i = 0; i < NARG; i++) {
        printf("Arg[%d]? ", i);
        fgets(buf, LENARG, stdin);
        if (buf[0] == '\n') break;
        buf[strlen(buf) - 1] = '\0';
        arglist[i] = strdup(buf);
    }
    arglist[i] = NULL;
    if (i > 0)
        execvp(arglist[0], arglist);
}
