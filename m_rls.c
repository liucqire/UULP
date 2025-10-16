#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

#define oops(m) {perror(m);exit(1);}
#define PORTNUM 15000

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: ./a.out hostname dirname");
        exit(1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        oops("socket");

    struct sockaddr_in servaddr;
    bzero((void*)&servaddr, sizeof(servaddr));
    struct hostent *hp = gethostbyname(argv[1]);
    if (hp == NULL)
        oops("gethostbyname");
    memmove(&servaddr.sin_addr, hp->h_addr, hp->h_length);
    servaddr.sin_port = htons(PORTNUM);
    servaddr.sin_family = AF_INET;

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
        oops("connect");
    if (write(sockfd, argv[2], strlen(argv[2])) != strlen(argv[2]))
        oops("write");
    if (write(sockfd, "\n", 1) != 1)
        oops("write");

    char recv[BUFSIZ];
    int nread;
    while ((nread = read(sockfd, recv, BUFSIZ)) > 0)
        if (write(1, recv, nread) != nread)
            oops("write");
    close(sockfd);
}
