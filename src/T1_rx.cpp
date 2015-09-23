/*
File : T1_rx.cpp
Author : LCIA
*/

#include "dcomm.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <stdio.h>

/* Delay to adjust speed of consuming uffer, in milliseconds */
#define DELAY 500

/* Define receive buffer size */
#define RXQSIZE 8

Byte rxbuf[RXQSIZE];
QTYPE rcvq = {0, 0, 0, RXQSIZE, rxbuf};
QTYPE *rxq = &rcvq;
Byte sent_xonxoff = XON;
bool send_xon = false, send_xoff = false;

/* Socket */
int sockfd; // listen on sock_fd

struct sockaddr_storage dmy;
socklen_t dmylen = sizeof (dmy);
/* Function Declaration */
static Byte *rcvchar(int sockfd, QTYPE *queue);
static Byte *q_get(QTYPE *, Byte *);


int main(int argc, char *argv[]) {

	if (argc!=2){
		printf("Argumen Salah\n");
		return 0;
	}
	char *port_number = argv[1];
	//Constructing local socket address
	struct addrinfo hints;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_protocol=IPPROTO_UDP;
	hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
	struct addrinfo* res =0;
	int err = getaddrinfo(NULL,port_number,&hints,&res);
	if (err!=0){
		printf("failed to resolve local socket address(err=%d)\n",err );
		return 0;
	}

	Byte C;

	//Creating Socket
	sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(sockfd==-1){
		printf("%s\n",strerror(errno));
		return 0l
	}	

	// Insert code here to bind socket to the port number given in argv[1].
	if(bind(sockfd,res->ai_addr,res->ai_addrlen)==-1)){
		printf("%s\n",strerror(errno) );
		return 0;
	}

	freeaddrInfo(res);

	/* Initialize XON/XOFF flags */
	/* Create child process */
	pid_t pid;
	pid = fork();


	/*** If Parrent Process ***/
	if (pid !=0){
		while (true){
			C = *(rcvchar(sockfd, rxq));
			
			/* Quit on end of file */
			if (C == Endfile) {
				exit(0);
			}
		}
	}
	
	/*** else If Child Process ***/
	else{
		while (true) { 
		/* Call q_get */ 
		/* Can introduce some delay here. */
		}
	}
	
	close(sockfd);

	return 0;
}

int count_buffer=0; //pengkitung banyak elemen di dala m buffer

Byte dumbuf[2];
static Byte *rcvchar(int sockfd, QTYPE *queue){
	/*
	Insert code here
	Read a character from socket and put it to receive buffer.
	If the number of characters in the receive buffer is above
	certain level, then send XOFF and set a flag.
	Return a poiinter to the buffer wher data is put.
	*/
	if (!send_xoff){
		ssize_t nReceived =recvfrom(sockfd, dumbuf,sizeof(dumbuf) ,0,(struct  sockaddr *) &dmy,&dmylen);
		if(nReceived < 0){
			printf("ERROR in recvfrom() \n");
		}
		else {
			queue->data[queue->rear] =  dumbuf[0];
			queue->count++;
			if (queue->rear < RXQSIZE -1 ){
				queue->rear++;
			}else{
				queue->rear=0;
			}
			count_buffer++;
		}
		if(dumbuf[0]!=Endfile && dumbuf[0]!=EF && dumbuf[0] != CR){
			printf("Menerima byte ke-%d.\n",count_buffer);
		}

		if(count_buffer>MIN_UPPERLIMIT && sent_xonxoff=XON){
			sent_xonxoff=XOFF;
			send_xon=false;
			send_xoff=true;
			printf("Buffer > minimum upperlimit. Mengirim XOFF\n");
			Byte dumbuf2[2]
			dumbuf2[0] = XOFF;
			ssize_t nSent = sendto(sockfd,dumbuf2,sizeof(dumbuf2),4,(struct  sockaddr *) &dmy,dmylen);
			if(nSent<0){
				printf("ERROR in sendto()\n");
			}
		}
		return &dumbuf[0];

	}
	else{
		Byte *dummy = 0;
		return dummy;
	}

}

/* q_get retuns a pointer to the buffer where data is read
 * or NULL if buffer is empty. */

static Byte *q_get(QTYPE *queue, Byte *data) {
	Byte *current;

	/* Nothing in the queue*/
	if(!queue -> count) return (NULL);

	/* Insert code here.
	Retrieve data from buffer, save it to "current" and "data"
	If the number of characters in the receive buffer is below
	certain level, then send XON.
	Increment front index and check for wraparound.
	*/

}
