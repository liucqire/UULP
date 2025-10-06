#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define DFL_PROMPT ">"
#define YES 1
#define NO 0

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

char *nextLine(char *, FILE *);
char **splitline(char *);
int execute(char **);
void freelist(char **);

int process(char **);
int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int syn_err(char *);

int main() {
    char *line;
    char **arglist;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    while ((line = nextLine(DFL_PROMPT, stdin)) != NULL) {
        if ((arglist = splitline(line)) != NULL) {
            process(arglist);
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
    
    line[siz] = '\0';
    if (c == EOF && siz == 0)
        return NULL;
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
    if (arglist == NULL)
        return 0;
    
    int pid;
    int child_info;
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(1);
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

int process(char **arglist) {
    int rv = 0;
    if (arglist[0] == NULL)
        rv = 0;
    else if (is_control_command(arglist[0]))
        rv = do_control_command(arglist);
    else if (ok_to_execute())
        rv = execute(arglist);
    return rv;
}

int ok_to_execute() {
    int rv = YES;
    if (if_state == WANT_THEN) {
        syn_err("then expected");
        rv = NO;
    } else if (if_state == THEN_BLOCK && if_result == SUCCESS) {
        rv = YES;
    } else if (if_state == THEN_BLOCK && if_result == FAIL) {
        rv = NO;
    }
    return rv;
}

int is_control_command(char *s) {
    return strcmp(s, "if") == 0 || strcmp(s, "fi") == 0 ||
           strcmp(s, "then") == 0 || strcmp(s, "else") == 0;
}

int do_control_command(char **arglist) {
    char *cmd = arglist[0];
    int rv = -1;
    
    if (strcmp(cmd, "if") == 0) {
        if (if_state != NEUTRAL)
            rv = syn_err("if unexpected");
        else {
            last_stat = process(arglist + 1);
            if_result = (last_stat == 0 ? SUCCESS : FAIL);
            if_state = WANT_THEN;
            rv = 0;
        }
    } else if (strcmp(cmd, "then") == 0) {
        if (if_state != WANT_THEN)
            rv = syn_err("then unexpected");
        else {
            if_state = THEN_BLOCK;
            rv = 0;
        }
    } else if (strcmp(cmd, "fi") == 0) {
        if (if_state != THEN_BLOCK)
            rv = syn_err("fi unexpected");
        else {
            if_state = NEUTRAL;
            rv = 0;
        }
    } else {
        perror("cmd error");
    }
    return rv;
}

int syn_err(char *s) {
    if_state = NEUTRAL;
    fprintf(stderr, "syntax error: %s\n", s);
    return -1;
}
