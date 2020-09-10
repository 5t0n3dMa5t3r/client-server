#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>

#define PORT 6666

int main()
{
	char recv_buff[1024];
	char send_buff[1024];  

	struct sockaddr_in server;
	int sock = 0;

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	printf("Enter the IP of the server you want to connect to :\n");
	char *ip;
	scanf("%s" , ip);
	server.sin_addr.s_addr = inet_addr(ip);

	//creating the socket

	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock<0)
	{
		printf("Could not create Socket!\n");
	}


	//starting a connection

	if(connect(sock, (struct sockaddr * )&server, sizeof(server))<0)
	{
		printf("Connection failed!\n");
		return 0;
	}

	printf("Connection Established!\n");

	//sending the name of the file

		
	printf("Enter the name of the file you want to recieve / Enter '""quit""' for closing the connection > \n");
	scanf("%s" , send_buff);

			

	char* file_name;
	file_name = send_buff;

	send(sock , send_buff , 1024 , 0);


	//closing the connection if client requested to quit

	if(strcmp(send_buff , "quit")==0)
	{
		printf("Closing the connection...\n");
		close(sock);
	    printf("Connection closed!\n");
		return 0;
	}

	//checking whether the file exists or not (acknowledgment)

	int ack = read(sock , recv_buff , 1);			


	if(strcmp(recv_buff , "0")==0)
	{
		printf("No such file exists!\n");
		close(sock);
		printf("Connection closed!\n");
		return 0;
	}


	//opening a file to store the data recieved


	FILE *file;
	file = fopen( file_name, "w");
	if(file==NULL)
	{
		printf("Could not open the file!\n");
		close(sock);
		printf("Connection closed!\n");
		return 0;
	}

	int bytes_received = 0;

	//receiving the file in chunks of 1024 bytes

	while(1)
	{
	bytes_received = read(sock , recv_buff , 1024);
				
		if(bytes_received>0)
		{
			printf("Number of Bytes received :%d\n", bytes_received );
			fwrite(recv_buff,1,bytes_received,file);	
		}

		if(bytes_received==0)
		{
			break;
		}		
	}

	//closing the file and the connection
	
	printf("File received , its stored in the local storage!\n");
	fclose(file);

	close(sock);
	printf("Connection closed!\n");
	return 0;


}
