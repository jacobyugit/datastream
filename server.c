/*
 * A simple datastream/TCP server in the Internet domain.
 *
 * The port number is passed as an argument
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUM	(2000)
#define BUFSIZE     (255)

/*
 * Error message
 */
void
err_msg( char *msg )
{
	perror( msg );
	exit( 1 );
}	/* err_msg */


/*
 * DOSTUFF()
 *
 * There is a separate instance of this function 
 * for each connection.  It handles all communication
 * once a connnection has been established.
 */
void
dostuff ( int sock )
{
	int n;
	char buffer[BUFSIZE + 1];
      
	bzero( buffer, BUFSIZE+1 );
	if ( (n = read(sock, buffer, BUFSIZE)) < 0 )
		err_msg( "ERROR reading from socket" );
	printf( "Here is the message: %s\n", buffer );

	if ( (n = write(sock, "I got your message",18)) < 0 )
		err_msg("ERROR writing to socket");
}	/* dostuff */


int
main( int argc, char *argv[] )
{
	int		pid;
	char	buffer[BUFSIZE + 1];

	int	sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	/* Check on port number to be used or use the default port number */
	if ( argc < 2 ) {
		fprintf( stdout, "%s: No port number provided\n", argv[0] );
		fprintf( stdout, "%s: Use default port number\n\n\n", argv[0] );
		portno = PORT_NUM;
	} else 
		portno = atoi( argv[1] );
	
	/* Create end point */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		err_msg("ERROR opening socket");

	/* Bind Internet address */
	bzero( (char *)&serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port			= htons(portno);
	if ( bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
		err_msg("ERROR on binding");

	/* Get ready to accept incoming connection */
	if ( listen(sockfd, 12) != 0 ) err_msg("ERROR on listen");

	clilen = sizeof( cli_addr );
	/* Enter server loop */
	while ( 1 ) {
		if ( (newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) < 0 )
			err_msg( "ERROR on accept" );

		if ( (pid = fork()) < 0 ) err_msg( "ERROR on fork" );

		/* On newly created child process... */
		if ( pid == 0 ){
			close( sockfd );
			dostuff( newsockfd );
			exit( 0 );
		}
		else	/* On parent process... */
			close( newsockfd );
	}	/* while */

	return( 0 ) ; 
}	/* main */
