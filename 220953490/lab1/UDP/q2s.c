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
    // printf("INPUT port number: ");
    // scanf("%d", &x);
    // getchar();  // Consume newline character after port number input
    x = 3388;
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
        printf("\nMessage received: Client sent the string -> %s\n", buff);

        if (!strcmp(buff, "halt")) {
            break;
        }

        int i = 0, j = strlen(buff) - 1, check=1;
        int arr[5] ={};

        while(i < j){
            if(buff[i] != buff[j]){
                check = 0;
                break;
            }
            i++;
            j--;
        }
        for(int i = 0 ; i < strlen(buff) ; i++){
            char c = tolower(buff[i]);
            if(c == 'a') arr[0]++;
            else if(c == 'e') arr[1]++;
            else if(c == 'i') arr[2]++;
            else if(c == 'o') arr[3]++;
            else if(c == 'u') arr[4]++;
        }

        // Prompt server for reply
        printf("\n\nType Message: ");

        // Send response back to client
        sntb = sendto(s, &check, sizeof(check), 0, (struct sockaddr*)&client, len);
        if (sntb == -1) {
            printf("\nMessage sending failed.");
            close(s);
            exit(0);
        }
        printf("\nMessage sent: palindrome check sent -> %d\n", check);
        sntb = sendto(s, arr, sizeof(arr), 0, (struct sockaddr*)&client, len);
        if (sntb == -1) {
            printf("\nMessage sending failed.");
            close(s);
            exit(0);
        }
        printf("\nMessage sent: vowel count array sent.\n");
    }

    close(s);
    return 0;
}
