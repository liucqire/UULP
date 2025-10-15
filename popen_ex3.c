#include <stdio.h>
#include <unistd.h>

int main() {
    FILE *fp;
    fp = popen("mail admin backup", "w");
    fprintf(fp, "Error with backup!!\n");
    pclose(fp);
}
