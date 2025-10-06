#include <unistd.h>
#include <stdio.h>

int main() {
    int rv, mypid;
    mypid = getpid();
    printf("Before: my pid is %d\n", mypid);
    rv = fork();
    sleep(1);
    printf("After: my pid is %d, fork() said %d\n", getpid(), rv);
}
