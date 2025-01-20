#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#define MAXSIZE 50

main()
{
	char buff[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return;
	}
	printf("CLIENT socket created (server port number -> 3388). \n");
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return;
	}
	printf("connected to server on port 3388. \n");
	for (i = 0; ; i+=1)
	{
		int n;
		printf("\nEnter the number of integers : ");
		scanf("%d",&n);

		int arr[n];
		printf("\nEnter the integers\n");
		for(int i  = 0 ; i < n ; i++){
            scanf("%d",&arr[i]);
		}

		sentbytes=send(sockfd,&n,sizeof(n),0);
		if(sentbytes==-1)
		{
			printf("!!");
			close(sockfd);
		}

		sentbytes=send(sockfd,arr,n*sizeof(int),0);
		if(sentbytes==-1)
		{
			printf("!!");
			close(sockfd);
		}

		int choice;
		printf("Enter your choice:\n");
        printf("1. Search for a number\n");
        printf("2. Sort numbers\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        sentbytes=send(sockfd,&choice,sizeof(choice),0);
		if(sentbytes==-1)
		{
			printf("!!");
			close(sockfd);
		}

        if(choice == 1){
            int num;
            int res;
            printf("\nWhich number do you want to search for ? : ");
            scanf("%d",&num);

            sentbytes=send(sockfd,&num,sizeof(num),0);
            if(sentbytes==-1)
            {
                printf("!!");
                close(sockfd);
            }

            recedbytes=recv(sockfd,&res,sizeof(res),0);
            if(res != -1){
                printf ("Message recieved : Number found at index %d !!\n", res);
            }else{
                printf ("Message recieved : Number not found \n");
            }
        }else if(choice == 2){
            int flag;
            printf("\nEnter 1 for Ascending order / 2 for Descending order.\n");
            scanf("%d", &flag);

            sentbytes=send(sockfd,&flag,sizeof(flag),0);
            if(sentbytes==-1)
            {
                printf("!!");
                close(sockfd);
            }

            recedbytes=recv(sockfd,arr,n * sizeof(int),0);
            printf("Message recieved : Sorted array -> ");
            for(int i  = 0 ; i < n ; i++){
                printf("%d\t",arr[i]);
            }
            printf("\n");

        }else{
            printf("invalid choice!!");
        }

        if(choice == 3){
            printf("EXITING!!\n");
            close(sockfd);
            break;
        }
	}
	close(sockfd);
}
