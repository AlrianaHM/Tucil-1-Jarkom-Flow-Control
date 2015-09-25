/*
File 	: transmitter.cpp
Author 	: Ignatius Alriana Haryadi Moel
		  Lucky Cahyadi Kurniawan
!! Dilarang mengcopy file ini sebagian atau seluruhnya !!
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
#include <fstream>
#include <iostream>

/* Delay to adjust speed of consuming uffer, in milliseconds */
#define DELAY 500

/* Define receive buffer size */
#define RXQSIZE 8

//Byte rxbuf[RXQSIZE];
//QTYPE rcvq = {0, 0, 0, RXQSIZE, rxbuf};
//QTYPE *rxq = &rcvq;
Byte sent_xonxoff = XON;
bool send_xon = false, send_xoff = false;

/* Socket */
int sockfd; // listen on sock_fd

struct sockaddr_storage dmy;
socklen_t dmylen = sizeof (dmy);
using namespace std;
int main(int argc, char *argv[]) {

	// Mengecek argumen
	if(argc < 3 || argc > 4)
			printf("transmitter [ip-address] [port-number] [file.txt]");

	// memasukan argument ke dalam local data
	char* ip_adr = argv[1];
	char* port_num = argv[2];
	char* filename = argv[3];

	//Constructing local socket address
	
	struct addrinfo hints;
	memset(&hints,0,sizeof(hints));
	hints.ai_socktype=SOCK_DGRAM;
	hints.ai_protocol=0;
	struct addrinfo* res =0;
	int err = getaddrinfo(ip_adr,port_num,&hints,&res);
	if (err!=0){
		printf("failed to resolve local socket address(err=%d)\n",err );
		return 0;
	}

	//Creating Socket
	sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(sockfd<0){
		printf("%s\n",strerror(errno));
		return 0;
	}	
/*
	// Insert code here to bind socket to the port number given in argv[1].
	if(bind(fd,res->ai_addr,res->ai_addrlen)==-1)){
		printf("%s\n",strerror(errno) );
		return 0;
	}
*/

	/* Initialize XON/XOFF flags */
	/* Create child process */
	pid_t pid;
	pid = fork();
	if (sent_xonxoff == XON)
		send_xon = true;
	else send_xoff = false;

	/*** If Parrent Process ***/
	if (pid !=0){
		while (true){
			// Menbaca File yang akan dikirim
			char ch;
			int i = 0;
			fstream fin("tes.txt", fstream::in);
			while (fin >> noskipws >> ch) {
				if (sent_xonxoff == XON){
	    			
	    			// Sending karakter/karakter
	    			ssize_t numBytes = sendto(sockfd, ch, 1, 0,res->ai_addr, res->ai_addrlen);
	    			if (numBytes < 0) cout << "sending failed\n" ;
	    			else {
	    				i++;
	    				cout << "Mengirim byte ke-"<< i <<": '"<< ch << "'\n";
	    			}

	    		} else cout << "Menunggu XON...\n";
			}
		}
	}
	
	/*** else If Child Process ***/
	else{
		while (true) {
			Byte dumchar[2];
			struct sockaddr_storage recAdrr;			// receiver adress
			socklen_t recAdrrlen = sizeof(recAdrr);		
		
			numBytes = recvfrom(sockfd, dumchar, size0f(dumchar), 0, (struct sockaddr *) &recAdrr, &recAdrrlen);
			if (numBytes < 0) cout << "receive failed \n";
			else if (dumchar[0] == XON){
				cout << "XON diterima\n";
				send_xoff = false;
				send_xon = true;
			} else if (dumchar[0] == XOFF){
				cout << "XOFF diterima\n";
				send_xoff = true;
				send_xon = false;
			}

		}
	}

	freeaddrinfo(res);

	close(sockfd);

	return 0;
}
