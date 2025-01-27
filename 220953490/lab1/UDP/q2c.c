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
    int sa;
    socklen_t len;
    // printf("INPUT port number: ");
    // scanf("%d", &x);
    // getchar();  // Consume newline character after port number input through scanf
    x = 3388;
    struct sockaddr_in server;
    char buff[1024];  //buffer size

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");

    server.sin_family = AF_INET;
    server.sin_port = htons(x);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    sa = sizeof(server);
    len = sizeof(server);

    while (1) {
        printf("\n\nString operations\n\n[enter \"halt\" to exit]\n\nEnter the String : ");
        //fgets
        fgets(buff, sizeof(buff), stdin);

        //Remove the newline character that fgets might leave at the end with a null temrinator 0
        buff[strcspn(buff, "\n")] = 0;

        //Send message to server
        sntb = sendto(s, buff, strlen(buff) + 1, 0, (struct sockaddr*)&server, len);
        if (sntb == -1) {
            close(s);
            printf("\nMessage sending failed.");
            exit(0);
        }

        if (!strcmp(buff, "halt"))
            break;

        //Receive response from the server
        int check = 0;
        recb = recvfrom(s, &check, sizeof(check), 0, (struct sockaddr*)&server, &sa);
        if (recb == -1) {
            printf("\nMessage receiving failed.");
            close(s);
            exit(0);
        }

        printf("\nMessage received: Palindrom check %d", check);
        if(check == 1) printf("Hence it is a palindrome !!\n");
        else printf("Hence it is not a palindorme!\n");
        
        int arr[5];
        recb = recvfrom(s, arr, sizeof(arr), 0, (struct sockaddr*)&server, &sa);
        if (recb == -1) {
            printf("\nMessage receiving failed.");
            close(s);
            exit(0);
        }
        printf("\nMessage received: vowel count array");
        printf("\n\nVOWEL COUNT\n1) 'a' -> %d\n2) 'e' -> %d\n3) 'i' -> %d\n4) 'o' -> %d\n5) 'u' -> %d\n",arr[0],arr[1],arr[2],arr[3],arr[4]);

        if (!strcmp(buff, "stop"))
            break;
    }

    close(s);
    return 0;
}
