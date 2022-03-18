#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
   
#include <pthread.h>
#include "thpool.h"
#include <fcntl.h>
#include <poll.h>


#define PORT     8080
#define CLIENT "127.0.0.1"
#define SIZE 1024
#define MAX_CLIENTS 10

struct Connecting {
    int sockfd;
    int foo;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
}; 


/*void calling(struct Connecting * structure){
    int len, n;
    char msg[512];
    

    len = sizeof(structure->cliaddr);  //len is value/resuslt
    
    n = recvfrom( structure->sockfd, 
		  (char *)msg, 
		  512, 
                  MSG_WAITALL, 
		  ( struct sockaddr *) &structure->cliaddr,
		  &len); //n is size of message
    if ( n > 0 )
	{
	msg[n] = '\0';
	for (int i = 0; i < n; i++)
        { 
     		printf("%d: %02X ",i, msg[i]);
	}
    	printf("\n");
	}
    printf("n:%d \n",n);
}*/

void receive_payload(struct Connecting * structure){
    
	struct pollfd pfds[1]; // More if you want to monitor more
    pfds[0].fd = structure->sockfd;          // Standard input
    pfds[0].events = POLLIN; 
		
    int len, n;
    char msg[512];
	
	printf("Hit RETURN or wait 2.5 seconds for timeout\n");
    int num_events = poll(pfds, 1, 10000); // 2.5 second timeout
	
	if (num_events == 0) {
		printf("Poll timed out!\n");
	} else {
		int pollin_happened = pfds[0].revents & POLLIN;
         
		if (pollin_happened) {
			printf("File descriptor %d is ready to read\n", pfds[0].fd);
		} else {
			printf("Unexpected event occurred: %d\n", pfds[0].revents);
		}
			
		read(pfds[0].fd, &msg, 18);
		//printf("%c",c);
		if ( msg[0]==0xAA){
				printf("THAT IS\n");
		}
		for (int i = 0; i < 18; i++)
		{ 
		 printf("%d: %02X ",i, msg[i]);
		}
		
	}

    /*len = sizeof(structure->cliaddr);  //len is value/resuslt
    n = recvfrom(structure->sockfd, 
				 (char *)msg, 
				 512, 
                 MSG_WAITALL, 
				 ( struct sockaddr *) &structure->cliaddr,
				 &len); //n is size of message
    msg[n] = '\0';
    for (int i = 0; i < n; i++)
    { 
     printf("%d: %02X ",i, msg[i]);
    }
    printf("\n");*/
}

    /*if(fcntl(structure->sockfd, F_SETFL, fcntl(structure->sockfd, F_GETFL) | O_NONBLOCK) < 0) {
    	if(fcntl(structure->sockfd, F_GETFL) & O_NONBLOCK) {
            calling(structure);


    //printf("\nI received confirm message: %s\n", msg);

    //calling(structure);
}   
*/

/*int wait_client(int server_socket)
{
    struct pollfd pollfds[MAX_CLIENTS + 1];
    pollfds[0].fd = server_socket;
    pollfds[0].events = POLLIN | POLLPRI;
    int useClient = 0;

    while (1)
    {
        // printf("useClient => %d\n", useClient);
        int pollResult = poll(pollfds, useClient + 1, 5000);
        if (pollResult > 0)
        {
            if (pollfds[0].revents & POLLIN)
            {
                struct sockaddr_in cliaddr;
				char buffer[512];

                int addrlen = sizeof(cliaddr);
                int client_socket = accept(server_socket, (struct sockaddr *)&cliaddr, &addrlen);
                //printf("accept success %s\n", inet_ntoa(cliaddr.sin_addr));
				
				int len = sizeof(cliaddr);  //len is value/resuslt
				int n = recvfrom( server_socket, 
								  (char *)buffer, 
								  512, 
								  MSG_WAITALL, 
								  ( struct sockaddr *) &cliaddr,
								  &len); //n is size of message
				buffer[n] = '\0';
            }
        }
    }
}*/

int main() {
    int sockfd;
    char buffer[512];
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    
	// Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { ///socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0)
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr(CLIENT);
    servaddr.sin_port = htons(PORT);
       
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    struct Connecting connect;
    connect.sockfd = sockfd;
    connect.servaddr.sin_family = servaddr.sin_family;
    connect.servaddr.sin_port = servaddr.sin_port;
    connect.servaddr.sin_addr.s_addr = servaddr.sin_addr.s_addr;
    connect.cliaddr = cliaddr;
    
	
	//int client_socket = wait_client(sockfd);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	printf("\naaaaa\n");
    
	receive_payload(&connect);

    /* Confirming message (sending)*/
    /*const char *confirm_msg = "OK";
    sendto(sockfd, (const char *)confirm_msg, strlen(confirm_msg), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("I sent confirm message!\n"); */
    //close(sockfd);
    //printf("I sent confirm message!\n"); 
	
    return 0;
}
