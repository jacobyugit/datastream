/*
 * A simple datastream/TCP client in the Internet domain.
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
#include <netdb.h> 

#define	BUFSIZE		(255)

void
err_msg( char *msg )
{
	perror( msg );

	exit( 0 );
}	/* err_msg */

int
main( int argc, char *argv[] )
{
	int		sockfd, portno, n;
	char	buffer[BUFSIZE + 1];

	struct	sockaddr_in serv_addr;
	struct	hostent *server;

	// Check for connection parameters
	if ( argc < 3 ) {
		fprintf( stderr, "usage: %s hostname port_number\n", argv[0] );
		exit( 1 );
	}

	// Create end point
    portno = atoi( argv[2] );
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) err_msg( "ERROR opening socket" );

	// Retrieve host info
	if ( (server = gethostbyname(argv[1])) == NULL ) {
		fprintf( stderr, "ERROR, no such host\n" );
		exit( 1 );
	}

	// Prepare to connect to the server
	bzero( (char *)&serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons(portno);
    if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) err_msg( "ERROR connecting" );

	// Messaging
	printf( "Please enter the message: " );
	bzero( buffer, BUFSIZE+1 );
	fgets( buffer, BUFSIZE, stdin );

	if ( (n = write(sockfd, buffer, strlen(buffer))) < 0 ) err_msg( "ERROR writing to socket" );

	bzero( buffer, BUFSIZE+1 );
	if ( (n = read(sockfd, buffer, BUFSIZE)) < 0 ) err_msg( "ERROR reading from socket" );
	printf( "%s\n", buffer );

	return( 0 );
}	/* main */
