#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define YES 1
#define NO 0
#define DEF_PROMPT ">"
#define is_delim(x) ((x) == ' ' || (x) == '\t')

void setup();
char *next_cmd(char *, FILE *);
char **splitline(char *);
int execute(char **);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void *, size_t);
char *newstr(char *, int);

int main() {
    char *cmdline, *prompt, **arglist;
    int result;
    prompt = DEF_PROMPT;
    setup();
    while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
        if ((arglist = splitline(cmdline)) != NULL) {
            result = execute(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }
    return 0;
}

void setup() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n) {
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}

int execute(char **argv) {
    int pid;
    int child_info = -1;
    if (argv[0] == NULL)
        return 0;
    if ((pid = fork()) == -1)
        perror("fork");
    else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("cannot execute command");
        exit(1);
    } else {
        if (waitpid(pid, &child_info, 0) == -1)
            perror("wait");
    }
    return child_info;
}

char *next_cmd(char *prompt, FILE *fp) {
    char *buf;
    int bufspace = 0;
    int pos = 0;
    int c;
    printf("%s", prompt);
    while ((c = getc(fp)) != EOF) {
        if (pos + 1 >= bufspace) {
            if (bufspace == 0)
                buf = emalloc(BUFSIZ);
            else 
                buf = erealloc(buf, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
        }
        if (c == '\n') 
            break;
        buf[pos++] = c;
    }
    if (c == EOF && pos == 0)
        return NULL;
    buf[pos] = '\0';
    return buf;
}

char **splitline(char *line) {
    char **args;
    int spots = 0;    
    int bufspace = 0;
    int argnum = 0;
    char *cp = line;
    char *start;
    int len;
    if (line == NULL)
        return NULL;

    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char *);
    
    while (*cp != '\0') {
        while (is_delim(*cp))
            cp++;
        if (*cp == '\0')
            break;
        if (argnum + 1 >= spots) {
            args = erealloc(args, bufspace + BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ / sizeof(char *));
        }
        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp)))
            len++;
        args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

char *newstr(char *s, int l) {
    char *rv = emalloc(l + 1);
    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char **list) {
    char **cp = list;
    while (*cp)
        free(*cp++);
    free(list);
}

void *emalloc(size_t n) {
    void *rv;
    if ((rv = malloc(n)) == NULL)
        fatal("out of memory", "", 1);
    return rv;
}

void *erealloc(void *p, size_t n) {
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", 1);
    return rv;
}

