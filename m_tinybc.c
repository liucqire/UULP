#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_dc(int *, int *);
void do_bc(int *, int *);

void fatal(char *msg, int en) {
    perror(msg);
    exit(en);
}

int main() {
    pid_t pid;
    int todc[2], fromdc[2];

    if (pipe(todc) == -1 || pipe(fromdc) == -1)
        fatal("pipe", 1);
    if ((pid = fork()) == -1)
        fatal("fork", 2);

    if (pid > 0)
        do_bc(todc, fromdc);
    else 
        do_dc(todc, fromdc);
}

void do_dc(int *in, int *out) {
    if (dup2(in[0], 0) == -1)
        fatal("dup2", 3);
    close(in[0]);
    close(in[1]);
    if (dup2(out[1], 1) == -1)
        fatal("dup2", 4);
    close(out[0]);
    close(out[1]);

    execlp("dc", "dc", NULL);
    fatal("fork", 5);
}

void do_bc(int *todc, int *fromdc) {
    int num1, num2;
    FILE *fpin, *fpout;
    char message[BUFSIZ], operation[BUFSIZ];
    if ((fpin = fdopen(todc[1], "w")) == NULL)
        fatal("fdopen", 6);
    if ((fpout = fdopen(fromdc[0], "r")) == NULL)
        fatal("fdopen", 7);
    close(todc[0]);
    close(fromdc[1]);

    while (printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL) {
        if (sscanf(message, "%d%[+-*/^]%d", &num1, operation, &num2) != 3)
            fatal("sscanf", 8);
        if (fprintf(fpin, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
            fatal("fprintf", 9);
        fflush(fpin);
        if (fgets(operation, BUFSIZ, fpout) == NULL)
            break;
        if (fputs(operation, stdout) == EOF)
            fatal("fputs", 11);
    }
    fclose(fpin);
    fclose(fpout);
}
