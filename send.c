#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int port = 8082;
    char *ip = "127.0.0.1";
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("error socket");
        exit(1);
    }
    printf("[+] socket connected\n");

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    n = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (n < 0)
    {
        perror("error connected");
        exit(1);
    }
    printf("connected to server\n");

    while (1)
    {
        bzero(buffer, 256);
        printf("enter message to server (type 'bye' to exit): ");
        fgets(buffer, 256, stdin);
        send(sockfd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "bye\n") == 0)
        {
            printf("Exiting...\n");
            break;
        }

        bzero(buffer, 256);
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("received from server: %s", buffer);
    }

    close(sockfd);

    return 0;
}
