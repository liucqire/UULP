#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        exit(10);
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("retval %d, %d\n", status >> 8, status);
    }
}
