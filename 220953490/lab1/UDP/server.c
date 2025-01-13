#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main() {
    int s, r, recb, sntb, x;
    int ca;
    socklen_t len;
    printf("INPUT port number: ");
    scanf("%d", &x);
    getchar();  // Consume newline character after port number input

    struct sockaddr_in server, client;
    char buff[1024];  // Increased buffer size to handle larger messages

    s = socket(AF_INET, SOCK_DGRAM, 0);
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

    r = bind(s, (struct sockaddr*)&server, sizeof(server));
    if (r == -1) {
        printf("\nBinding error.");
        exit(0);
    }
    printf("\nSocket bound.");

    while (1) {
        // Receive message from client
        recb = recvfrom(s, buff, sizeof(buff), 0, (struct sockaddr*)&client, &ca);
        if (recb == -1) {
            printf("\nMessage receiving failed.");
            close(s);
            exit(0);
        }
        printf("\nMessage received: %s", buff);

        if (!strcmp(buff, "stop")) {
            break;
        }

        // Prompt server for reply
        printf("\n\nType Message: ");
        // Use fgets to allow spaces in the message
        fgets(buff, sizeof(buff), stdin);

        // Remove the newline character that fgets might leave at the end
        buff[strcspn(buff, "\n")] = 0;

        // Send response back to client
        sntb = sendto(s, buff, strlen(buff) + 1, 0, (struct sockaddr*)&client, len);
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
