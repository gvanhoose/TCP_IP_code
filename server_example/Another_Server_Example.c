#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void error( char* message, int exit_value )
{
	perror( message );
	exit( exit_value );
}

int main( int argc, char* argv[] )
{
	int sockFD, newSockFD, portno, clilen;

	char buffer[ 256 ];

	struct sockaddr_in* serv_addr;
	struct sockaddr_in* cli_addr;

	int n;

	if( argc < 2 )
	{
		fprintf( stderr, "ERROR, no port provided\n" );
		exit( 1 );
	}

	sockFD = socket( AF_INET, SOCK_STREAM, 0 );

	if( sockFD < 0 )
	{
		error( "ERROR opening socket", -1 );
	}

//	bzero( (char*) serv_addr, sizeof( serv_addr ) );	// \_ changed from online example
	memset( (char*) serv_addr, '\0', sizeof( serv_addr ) );	// /

	portno = atoi( argv[1] );

	serv_addr->sin_family = AF_INET;

	serv_addr->sin_addr.s_addr = INADDR_ANY;

	serv_addr->sin_port = htons( portno );

	if( bind( sockFD, (struct sockaddr*) serv_addr, sizeof( serv_addr) ) < 0)
	{
		error( "ERROR on binding", -1 );
	}

	listen( sockFD, 5 );

	clilen = sizeof( cli_addr );

	newSockFD = accept( sockFD, (struct sockaddr*) cli_addr, &clilen );

	if( newSockFD < 0 )
	{
		error( "ERROR on accept", -1 );
	}

//	bzero( buffer, 256 );
	memset( buffer, '\0', 256 );

	n = read( newSockFD, buffer, 256 );

	if( n < 0 )
	{
		error( "ERROR reading from socket", -1 );
	}

	printf( "Received:\n\t\t\"%s\"\n", buffer );

	n = write( newSockFD, "I got your message", 18 );

	if( n < 0)
	{
		error( "ERROR writing to client socket", -1 );
	}

	return 0;
}
