#include <asm-generic/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>

#define PORTNUM 15000
#define oops(m) {perror(m);exit(1);}

void deal(char *str) {
    char *l, *r;
    for (l = r = str; *r; r++)
        if (*r == '/' || isalnum(*r) || *r == '.')
            *l++ = *r;
    *l = '\0';
}

int main(int argc, char **argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == 0)
        oops("socket");

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    
    struct sockaddr_in servaddr;
    char hostname[BUFSIZ];
    gethostname(hostname, BUFSIZ);
    struct hostent *hp = gethostbyname(hostname);
    if (hp == NULL) 
        oops("gethostbyname");
    printf("hostname:%s\n", hostname);
    bzero(&servaddr, sizeof(servaddr));
    memmove(&servaddr.sin_addr, hp->h_addr, hp->h_length);
    servaddr.sin_port = htons(PORTNUM);
    servaddr.sin_family = AF_INET;

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
       oops("bind");
    if (listen(sockfd, 1) != 0)
       oops("listen");
    
    printf("now start accepting\n");
    while (1) {
        int clifd;
        if ((clifd = accept(sockfd, NULL, NULL)) < 0)
            oops("accept");
        printf("receive a connection\n");
        FILE *cli_i = fdopen(clifd, "r");
        if (cli_i == NULL)
            oops("fdopen");
        FILE *cli_o = fdopen(clifd, "w");
        if (cli_o == NULL)
            oops("fdopen");
        fputs("connected\n", cli_o);
        
        char dirname[BUFSIZ - 10];
        if (fgets(dirname, BUFSIZ - 10, cli_i) == NULL)
            oops("fgets");
        printf("before deal with it %s", dirname);
        
        deal(dirname);
        printf("dirname is %s\n", dirname);
        char cmd[BUFSIZ];
        snprintf(cmd, BUFSIZ, "ls %s", dirname);
        
        printf("cmd:%s\n", cmd);
        printf("about to run ls\n");
        FILE *pp = popen(cmd, "r");
        if (pp == NULL)
            oops("popen");
        printf("ls done\n");

        int c;
        while ((c = getc(pp)) != EOF) {
            putc(c, cli_o);
            putc(c, stdout);
        }
        fflush(cli_o);

        pclose(pp);
        fclose(cli_i);
        fclose(cli_o);
    } 
}
