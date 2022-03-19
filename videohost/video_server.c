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

//#include <gst/gst.h>



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
		printf("\n");
	}
}

void calling(struct Connecting * structure){
	//g_timeout_add (10000, receive_payload, &structure);
	//threadpool thpool = thpool_init(1);
    //thpool_add_work(thpool, (void*)receive_payload, structure);
    //thpool_wait(thpool);
    //thpool_destroy(thpool);
    receive_payload(structure);
}

/*  if(fcntl(structure->sockfd, F_SETFL, fcntl(structure->sockfd, F_GETFL) | O_NONBLOCK) < 0) {
    	if(fcntl(structure->sockfd, F_GETFL) & O_NONBLOCK) {
            calling(structure);


    //printf("\nI received confirm message: %s\n", msg);

    //calling(structure);  
*/
struct Connecting establish_connection(){
	
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { ///socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0)
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
	
	servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr(CLIENT);
    servaddr.sin_port = htons(PORT);
	
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
	
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);
	//printf("\naaaaa\n");
	
	return connect;
}

int main() {
    
	/*threadpool thpool = thpool_init(1);
    thpool_add_work(thpool, (void*)calling, &connect);
    thpool_wait(thpool);
    thpool_destroy(thpool);*/
	struct Connecting connect;
    connect = establish_connection();
	calling(&connect);
	
	printf("aaa\n");

	//receive_payload(&connect);

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
