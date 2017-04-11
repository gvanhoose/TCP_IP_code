#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main( int argc, char* argv[] )
{
	int sockFD = 0, n = 0;

	char recBuff[ 1024 ];

	struct sockaddr_in* serv_addr;

	if( argc != 2 )
	{
		printf( "\nUsage:  \"%s <IP of server>\"\n", argv[0] );
		return -1;
	}

	memset( recBuff, '\0', sizeof( recBuff ) );

	if( (sockFD = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		printf( "Error:  Could not create socket\n" );
		return -1;
	}

	memset( serv_addr, '\0', sizeof( serv_addr ) );

	serv_addr->sin_family = AF_INET;

	serv_addr->sin_port = htons( 5000 );

	if( inet_pton( AF_INET, argv[ 1 ], serv_addr->sin_addr ) <= 0 )
	{
		printf( "inet_pton error occurred\n" );
		return -1;
	}

	if( connect( sockFD, (struct sockaddr*) serv_addr, sizeof( serv_addr ) ) < 0 )
	{
		printf( "Error:  Connect Failed\n" );
		return -1;
	}

	while( (n = read( sockFD, recBuff, sizeof( recBuff) -1 )) > 0 )
	{
		recBuff[ n ] = 0;

		if( fputs( recBuff, stdout ) == EOF )
		{
			printf( "Error:  fputs error\n" );
		}
	}

	if( n < 0 )
	{
		printf( "Read error\n" );
	}

	return 0;
}
