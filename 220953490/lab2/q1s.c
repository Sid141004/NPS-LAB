#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void reorderString(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void replaceAll(char *str, const char *oldWord, const char *newWord)
{
    char *pos, temp[1000];
    int index = 0;
    int owlen;

    owlen = strlen(oldWord);
    while ((pos = strstr(str, oldWord)) != NULL)
    {
        strcpy(temp, str);
        index = pos - str; //strstr return a memory address.
        str[index] = '\0'; // terminates the string at the index where word was found
        strcat(str, newWord);// appends new word
        strcat(str, temp + index + owlen); // appends rest of the orginal string
    }
}

int main()
{
    int sfd, new_socket;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buff[1000];

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("Error in socket creation");
        exit(1); // Use a non-zero exit code to indicate an error.
    }
    else
    {
        printf("Socket created successfully\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3389);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error in binding");
        exit(1); // Use a non-zero exit code to indicate an error.
    }
    else
    {
        printf("Binded successfully\n");
    }

    if (listen(sfd, 5) < 0)
    {
        perror("Error in listen");
        exit(1);
    }

    clilen = sizeof(cliaddr);

    while (1)
    {
        new_socket = accept(sfd, (struct sockaddr *)&cliaddr, &clilen);
        if (new_socket < 0)
        {
            perror("Error in accept");
            continue;;
        }
        char filename[100];
        recv(new_socket, filename, sizeof(filename), 0);
        printf("Filename received: %s\n", filename);
        FILE *fp = fopen(filename, "r");
        FILE *tmpfile;
        if (fp == NULL)
        {
            send(new_socket, "File not found", sizeof("File not found"), 0);
            printf("\nMESSAGE SENT: File not found !!\n");
            fclose(fp);
            close(new_socket);
            continue;
        }     
        else
        {   
            send(new_socket, "File found", sizeof("File found"), 0);
            printf("\nMESSAGE SENT: File found !!\n");
            fclose(fp);
            while(1){
                int choice;
                recv(new_socket, &choice, sizeof(choice), 0);
                if (choice == 1)
                {
                    printf("\nMESSAGE RECIEVED: Choice 1!!\n");
                    fp = fopen(filename, "r");
                    char search[100];
                    recv(new_socket, search, sizeof(search), 0);
                    char line[100];
                    int occurrences = 0;
                    while (fgets(line, sizeof(line), fp))//to read a line from file
                    {
                        char *ptr = line;//points to starting of line
                        while ((ptr = strstr(ptr, search)))//to find first occurance
                        {
                            occurrences++;
                            ptr++;//points to next char
                        }
                    }
                    fclose(fp);
                    send(new_socket, &occurrences, sizeof(occurrences), 0);
                    printf("\nMESSAGE SENT: occurences : %d!\n", occurrences);
                }
                else if (choice == 2)
                {
                    char search[100];
                    char replace[100];
                    recv(new_socket, search, sizeof(search), 0);
                    recv(new_socket, replace, sizeof(replace), 0);
                    printf("\nMessage Recieved : Replacing %s with %s\n", search, replace);
                    fp = fopen(filename, "r");
                    char line[100];
                    tmpfile = fopen("temp.txt", "w");
                    while (fgets(line, sizeof(line), fp))
                    {
                        replaceAll(line, search, replace);
                        fputs(line, tmpfile);
                    }
                    fclose(fp);
                    fclose(tmpfile);
                    remove(filename);
                    rename("temp.txt", filename);
                    send(new_socket, "Replaced", sizeof("Replaced"), 0);
                    printf("\nMESSAGE SENT: replaced !!\n");
                }
                else if (choice == 3)
                {
                    char line[100];
                    tmpfile = fopen("temp.txt", "w");
                    fp = fopen(filename, "r");
                    while (fgets(line, sizeof(line), fp))
                    {
                        reorderString(line);
                        fputs(line, tmpfile);
                    }
                    fclose(fp);
                    fclose(tmpfile);
                    remove(filename);
                    rename("temp.txt", filename);
                    send(new_socket, "Reordered", sizeof("Reordered"), 0);
                    printf("\nMESSAGE SENT: reordered !!\n");
                }
            }
        }
        close(new_socket);
    }
    close(sfd);
    return 0;
}
