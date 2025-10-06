#include "smsh.h"

int main() {
    char *cmdline, *prompt, **arglist;
    int result;
    prompt = DFL_PROMPT;
    
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
        if ((arglist = splitline(cmdline)) != NULL) {
            result = execute(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }

    return 0;
}

void fatal(char *s1, char *s2, int n) {
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(0);
}
