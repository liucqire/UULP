#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void tty_mode(int x) {
    static struct termios bak;
    static int s = 0;
    if (x == 0) {
        tcgetattr(0, &bak);
        s = 1;
    } else if (s == 1) {
        tcsetattr(0, TCSANOW, &bak);
    }
}

void f(int signo) {
    tty_mode(1);
    exit(1);
}

int main() {
    int c;
    struct termios info;
    tty_mode(0);
    signal(SIGINT, f);
    tcgetattr(0, &info);
    info.c_lflag &= ~ECHO;
    info.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &info);
    while ((c = getchar()) != EOF) {
        if (c == 'q' || c == 'Q')
            break;
        if (c == 'z')
            c = 'a';
        else if (islower(c))
            c++;
        putchar(c);
    }
    tty_mode(1);
}
