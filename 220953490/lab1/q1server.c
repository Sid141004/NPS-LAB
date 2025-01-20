#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 90

main()
{
	int sockfd,newsockfd,retval,i;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE];

	int a=0;
	int choice,num;
	int j=0;

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd==-1)
	{
	printf("\nSocket creation error");
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==1)
	{
		printf("Binding error");
		close(sockfd);
	}
	printf("Socket bound.\n");
    printf("\nserver is listening\n");
	retval=listen(sockfd,1);
	if(retval==-1)
	{
	close(sockfd);
	}
	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(newsockfd==-1)
	{
		close(sockfd);
	}

	for (i = 0; ; i+=1)
	{
        int n;
		recedbytes=recv(newsockfd,&n,sizeof(n),0);
		if(recedbytes==-1)
		{
			close(sockfd);
			close(newsockfd);
		}

        int arr[n];
        recedbytes=recv(newsockfd,arr,n * sizeof(int),0);
		if(recedbytes==-1)
		{
			close(sockfd);
			close(newsockfd);
		}

		recedbytes=recv(newsockfd,&choice,sizeof(choice),0);
		if(recedbytes==-1)
		{
			close(sockfd);
			close(newsockfd);
		}

		if(choice == 1){
            recedbytes=recv(newsockfd,&num,sizeof(num),0);
            printf("\nMessage recieved : client chose number - > %d \n", num);
            int res = -1;
            for(int i = 0 ;i < n ; i++){
                if(arr[i] == num){
                    res = i;
                }
            }
            sentbytes = send(newsockfd,&res,sizeof(res),0);
            if(sentbytes==-1)
            {
                close(sockfd);
                close(newsockfd);
            }
		}else if(choice == 2){
            // Sorting numbers
            int sort;
            // Receive the sort choice (ascending or descending)
            recedbytes = recv(newsockfd, &sort, sizeof(sort), 0);
            if (recedbytes <= 0) {
                printf("Recv Error\n");
                break;
            }
            printf("\nMessage recieved : client chose sort order - > %d \n", sort);

            if (sort == 1) {
                // Sorting in ascending order
                printf("\nSorting in ascending order...\n");
                for (i = 0; i < n - 1; i++) {
                    for (j = 0; j < n - 1 - i; j++) {
                        if (arr[j] > arr[j + 1]) {
                            int temp = arr[j];
                            arr[j] = arr[j + 1];
                            arr[j + 1] = temp;
                        }
                    }
                }
                sentbytes = send(newsockfd, arr, n * sizeof(int), 0);
                if (sentbytes == -1) {
                    printf("Send error\n");
                    break;
                }
            }
            else if (sort == 2) {
                // Sorting in descending order
                printf("\nSorting in descending order...\n");
                for (i = 0; i < n - 1; i++) {
                    for (j = 0; j < n - 1 - i; j++) {
                        if (arr[j] < arr[j + 1]) {
                            int temp = arr[j];
                            arr[j] = arr[j + 1];
                            arr[j + 1] = temp;
                        }
                    }
                }
                sentbytes = send(newsockfd, arr, n * sizeof(int), 0);
                if (sentbytes == -1) {
                    printf("Send error\n");
                    break;
                }
            }
            else {
                printf("Invalid sort choice\n");
            }
		}
		else if (choice == 3) {
            // Exit condition
            break;
        }
        else {
            printf("Invalid choice\n");
            break;
        }
	}
	close(sockfd);
	close(newsockfd);
}

