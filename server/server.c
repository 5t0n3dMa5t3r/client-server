#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>
#include <dirent.h>

#define PORT 6666

int search();

int main()
{
	
	char recv_buff[1024];
	char send_buff[1024];

	int socketId ,bindId, client;
	struct sockaddr_in server;
 	int addLen = sizeof(server);

 	server.sin_family = AF_INET;
 	server.sin_port = htons(PORT);
 	server.sin_addr.s_addr = inet_addr("192.168.1.5");

 	//creating the socket

 	socketId = socket(AF_INET , SOCK_STREAM , 0);

 	if(socketId<0)
 	{
 		printf("Could not create Socket!\n");
 	}


 	//binding the Port

 	bindId = bind(socketId , (struct sockaddr * )&server, addLen);
 	
 	if(bindId<0)
 	{
 		printf("Could not bind the Port with the address!\n");
 	}

 	//listening on the port

 	if(listen(socketId , 0)<0)
 	{
 		printf("Listening failed!\n");
 	}


 	printf("Listening on PORT:9001\n");

 	//accepting the connection

 	client = accept(socketId , (struct sockaddr *)&server, (socklen_t *)&addLen);
 	printf("Connection Established!\n");

 	//reading the client request

    printf("Reading form the client!\n");
    read(client, recv_buff , 1024);
    printf("Client: %s\n", recv_buff);

    //closing the connection if client requested to quit

    if(strcmp(recv_buff , "quit")==0)
    {
        printf("Closing the connection...(requested by client)\n");
        close(socketId);
        printf("Connection closed!\n");
        return 0;
    }   
    
    
    //closing the connection if client requested a non existent file
    
    if(search(recv_buff)==0)
    {
        printf("Client requested a non existent file!\n");
        write(client , "0", 1 );
        close(socketId);
        printf("Connection closed!\n");
        return 0;
    }

    write(client , "1" ,1);

    //opening the file requested by client

    FILE *file;
    file = fopen(recv_buff, "r");
    if(file==NULL)
    {
        printf("Could not open file\n");
        close(socketId);
        printf("Connection closed!\n");
        return 0;

    }

    //sending the file in chunks of 1024 bytes

    while(1)
    {
        int bytes_read = fread(send_buff , 1 , 1024 , file);
    

        if(bytes_read>0)
        {
            printf("Number of bytes read from the file:%d\n",bytes_read );
            printf("Sending %d bytes to the client!\n" , bytes_read);
            write(client , send_buff , bytes_read);
        }

        if(bytes_read==0)
        {
            break;
        }
    
    }
    
    //closing the file and the connection

    printf("File sent!\n");
    fclose(file);



 	close(socketId);
    printf("Connection closed!\n");
 	return 0;

}



int search(char* filename)  		//searches for the file in the present working directory of the server. ofcourse!
{
		
	DIR *d;
    struct dirent *dir;
    d = opendir(".");

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name,filename)==0)
            {
            	return 1;
            }
        }
        
        closedir(d);
    }
    
    return(0);
}
