#include <stdio.h>
#include <unistd.h>

int main() {
    char buf[BUFSIZ];
    FILE *fp = popen("who|sort", "r");
    while (fgets(buf, BUFSIZ, fp) != NULL)
        printf("%s", buf);
    pclose(fp);
}
