#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error( char* message, int exitValue )
{
	perror( message );
	exit( exitValue );
}

int main( int argc, char* argv[] )
{
	int sockFD, portNo, n;

	struct sockaddr_in* serv_addr;

	struct hostent* server;

	char buffer[ 256 ];

	if( argc < 3 )
	{
		fprintf( stderr, "Usage:  \"%s <hostname> <port>\"\n", argv[0] );
		exit( 0 );
	}

	portno = atoi( argv[ 2 ] );

	sockFD = socket( AF_INET, SOCK_STREAM, 0 );

	if( sockfd < 0 )
	{
		error( "ERROR opening socket", -1 );
	}

	server = gethostbyname( argv[1] );

	if( server == NULL )
	{
		fprintf( stderr, "ERROR, no such host\n" );
		exit( 0 );
	}

//	bzero( (char*) serv_addr, sizeof( serv_addr ) );	// \_ converted to memset call
	memset( (char*) serv_addr, '\0', sizeof( serv_addr ) );	// /

	serv_addr->sin_family = AF_INET;

	bcopy( (char*) server->h_addr, (char*) serv_addr->sin_addr.s_addr, server->h_length );

	serv_addr->sin_port = htons( portno );

	if( connect( sockFD, (struct sockaddr*) serv_addr, sizeof( serv_addr) ) < 0 )
	{
		error( "ERROR connecting", -1 );
	}

	printf( "Please enter the message:  " );

//	bzero( buffer, 256 );
	memset( buffer, '\0', 256 )

	fgets( buffer, 255, stdin );

	n = write( sockFD, buffer, strlen( buffer ) );

	if( n < 0 )
	{
		error( "ERROR writing to socket", -1 );
	}

//	bzero( buffer, 256 );
	memset( buffer, '\0', 256 );

	n = read( sockFD, buffer, 255 );

	if( n < 0 )
	{
		error( "ERROR reading from socket", -1 );
	}

	printf( "Received:\n\t\"%s\"\n", buffer );

	return 0;
}
