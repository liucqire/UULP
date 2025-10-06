#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void f(int);

int main() {
    int i;
    signal(SIGINT, f);
    for (int i = 0; i < 5; i++) {
        printf("HELLO\n");
        sleep(1);
    }
}

void f(int signo) {
    printf("OUCH!\n");
}
