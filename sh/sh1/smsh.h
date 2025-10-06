#include <stdio.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define YES 1
#define NO 0
#define DFL_PROMPT ">" 

char *next_cmd(char *, FILE*);
char **splitline(char *);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void *, size_t);
int execute(char **);
void fatal(char *, char *, int);
