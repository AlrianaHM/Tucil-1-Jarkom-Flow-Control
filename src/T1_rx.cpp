/*
File : T1_rx.cpp
Author : LCIA
*/

#include "dcomn.h"

/* Delay to adjust speed of consuming uffer, in milliseconds */
#define DELAY 500

/* Define receive buffer size */
#define RXQSIZE 8

Byte rxbuf[RXQSIZE];
QTYPE rcvq = {0, 0, 0, RXQSIZE, rxbuf};
QTYPE *rxq = &rcvg;
Byte sent_xonxoff = XON;
bool send_xon = false, send_xoff = false;

/* Socket */
int sockfd; // listen on sock_fd

/* Function Declaration */
static Byte *rcvchar(int sockfd, QTYPE *queue);
static Byte *q_get(QTYPE *, Byte *);

int main(int argc, char *argv[]) {

	Byte C;
	// Insert code here to bind socket to the port number given in argv[1].

	/* Initialize XON/XOFF flags */
	/* Create child process */

	/*** If Parrent Process ***/
	while (true){
		c = *(rcvchar(sockfd, rxq));
		
		/* Quit on end of file */
		if (c == Endfile) {
			exit(0);
		}
	}
	/*** else If Child Process ***/
	while (true) { 
	/* Call q_get */ 
	/* Can introduce some delay here. */
	}
}

static Byte *rcvchar(int sockfd, QTYPE *queue){

	/*
	Insert code here
	Read a character from socket and put it to receive buffer.
	If the number of characters in the receive buffer is above
	certain level, then send XOFF and set a flag.
	Return a poiinter to the buffer wher data is put.
	*/
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
