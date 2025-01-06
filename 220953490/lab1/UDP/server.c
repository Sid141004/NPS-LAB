#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int s, r, recb, sntb, x;
    int ca;
    printf("INPUT port number: ");
    scanf("%d", &x);
    socklen_t len;
    struct sockaddr_in server, client;
    char buff[50];

    s = socket(AF_INET, SOCK_DGRAM, 0);//0 means default protocol . for dgram , it is UDP by default
    if (s == -1) {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");

    server.sin_family = AF_INET;
    server.sin_port = htons(x);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    len = sizeof(client);
    ca = sizeof(client);

    r = bind(s, (struct sockaddr*)&server, sizeof(server));//(s is socket, IP address+port number , size of address?)
    if (r == -1) {
        printf("\nBinding error.");
        exit(0);
    }
    printf("\nSocket bound.");

    while (1) {
        recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, &ca);
        if (recb == -1) {
            printf("\nMessage receiving failed.");
            close(s);
            exit(0);
        }
        printf("\nMessage received: ");
        printf("%s", buff);

        if (!strcmp(buff, "stop")) {
            break;
        }

        printf("\n\nType Message: ");
        scanf("%s", buff);

        sntb = sendto(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, len);
        if (sntb == -1) {
            printf("\nMessage sending failed.");
            close(s);
            exit(0);
        }

        if (!strcmp(buff, "stop")) {
            break;
        }
    }

    close(s);
    return 0;
}
