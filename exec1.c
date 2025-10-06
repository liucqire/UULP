#include <stdio.h>
#include <unistd.h>

int main() {
    char *arglist[] = {"ls", "-l", 0};
    printf("*** About to exec ls -l\n");
    execvp("ls", arglist);
    printf("*** ls is done. bye\n");
}
