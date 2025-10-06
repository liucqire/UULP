#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define DFL_PROMPT ">"

char *nextLine(char *, FILE *);
char **splitline(char *);
int execute(char **);
void freelist(char **);

int main() {
    char *line;
    char **arglist;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    while ((line = nextLine(DFL_PROMPT, stdin)) != NULL) {
        if ((arglist = splitline(line)) != NULL) {
            execute(arglist);
            freelist(arglist);
        }
        free(line);
    }
    puts("");
}

char *nextLine(char *p, FILE *fp) {
    char *line;
    int cap = BUFSIZ;
    int siz = 0;
    int c;
    line = malloc(BUFSIZ);
    
    printf("%s", p);
    while ((c = getc(fp)) != EOF) {
        if (siz + 1 >= cap) {
            cap += BUFSIZ;
            line = realloc(line, cap);
        }
        if (c == '\n')
            break;
        line[siz++] = c;
    } 
    
    if (c == EOF && siz == 0)
        return NULL;
    line[siz] = '\0';
    return line;
}

char **splitline(char *line) {
    if (line == NULL)
        return NULL;

    char *cp = line;
    char **arglist;
    int nbytes = BUFSIZ;
    int spots = nbytes / sizeof(char *);
    int used = 0;
    arglist = malloc(BUFSIZ + 1);

    while (*cp != '\0') {
        while (isspace(*cp))
            cp++;
        if (*cp == '\0')
            break;
        if (used + 1 >= spots) {
            nbytes += BUFSIZ;
            spots += BUFSIZ / sizeof(char *);
            arglist = realloc(arglist, nbytes);
        
        }
        char *start = cp;
        int len = 0;
        while (!isspace(*cp) && *cp != '\0') {
            len++;
            cp++;
        }
        
        char *newstr = malloc(sizeof(char) * (len + 1));
        strncpy(newstr, start, len);
        newstr[len] = '\0';
        arglist[used++] = newstr;
    }
    
    arglist[used] = NULL;
    return arglist;
}

int execute(char **arglist) {
    if (arglist[0] == NULL)
        return 0;
    
    int pid;
    int child_info;
    if ((pid = fork()) == -1) {
        perror("fork");
    } else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(arglist[0], arglist);
    } else {
        waitpid(pid, &child_info, 0);
    }
    return child_info;
}

void freelist(char **list) {
    char **cp = list;
    while (*cp) {
        free(*cp);
        cp++;
    }
    free(list);
}
