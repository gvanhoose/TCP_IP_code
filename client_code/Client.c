#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT_NUMBER 5000
#define NULL_CHAR '\0'
#define BUFFER_LENGTH 128
#define LACCEPT_ERROR -1
#define LWRITE_ERROR -2
#define LSOCKET_ERROR -3
#define LINET_PTON_ERROR -4
#define SERVER_IP_ADDRESS "192.168.2.1"

void dbug()
{
	printf( "Reaching\n" );
}

int openSocket( struct sockaddr_in sockAddr )
{
	int senderFD;

	if( (senderFD = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
	{
		return LSOCKET_ERROR;
	}

	memset( (char*) &sockAddr, NULL_CHAR, sizeof( sockAddr ) );

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	sockAddr.sin_port = htons( PORT_NUMBER );	// if there's a bug and you can't quite figure it out, htons may not be neccessary here!

	if( inet_pton( AF_INET, SERVER_IP_ADDRESS, &sockAddr.sin_addr ) <= 0 )
	{
		return LINET_PTON_ERROR;
	}

	return senderFD;

}

int sendInstruction( const char* instruction, int senderFD, struct sockaddr_in sockAddr )
{
	int  serverFD;

	if( (serverFD = connect( senderFD, (struct sockaddr*) &sockAddr, sizeof( sockAddr ) )) < 0 )
	{
		return LACCEPT_ERROR;
	}

	int bytesWrittenCount;

	if( (bytesWrittenCount = write( senderFD, instruction, strlen( instruction ) )) < 0 )
	{
		close( serverFD );
		return LWRITE_ERROR;
	}

	close( serverFD );

	return bytesWrittenCount;
}

int main()
{

	struct sockaddr_in senderAddr;

	int senderFD;

	if( (senderFD = openSocket( senderAddr )) < 0)
	{
		switch( senderFD )
		{
			case LSOCKET_ERROR:
			{
				printf( "socket error\n" );
				return -1;
			}
			case LINET_PTON_ERROR:
			{
				printf( "inet_pton error\n" );
				return -1;
			}
			default:
			{
				printf( "undefined error: %d\n", senderFD );
				return -1;
			}
		}
	}

	int bytesWrittenCount;

	if( (bytesWrittenCount = sendInstruction( "test0", senderFD, senderAddr )) < 0 )
	{
		switch( bytesWrittenCount )
		{
			case LACCEPT_ERROR:
			{
				printf( "accept error\n" );
				return -1;
			}
			case LWRITE_ERROR:
			{
				printf( "write error" );
				return -1;
			}
			default:
			{
				printf( "undefined error during attempt to write to server.\n" );
				return -1;
			}
		}
	}

	return 0;
}
