#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP_ADDRESS "10.1.1.1"

int main()
{
	int socket_FD;

	struct sockaddr_in server;
	char* data;

	printf( "client:\tprepare for socket\n" );

	if( (socket_FD = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		printf( "client:\tsocket error\n" );
		return -1;
	}

	printf( "client:\tsuccess\n" );

	server.sin_addr.s_addr = inet_addr( IP_ADDRESS );
	server.sin_family = AF_INET;
	server.sin_port = htons( 5000 );

	printf( "client:\tprepare for connect\n" );

	if( connect( socket_FD, (struct sockaddr*) &server, sizeof( server ) ) < 0 )
	{
		printf( "client:\tconnect error\n" );
		return -1;
	}

	printf( "client:\tsuccess\n" );

	data = "Hello World!\n";

	printf( "client:\tprepare to send data\n" );

	if( send( socket_FD, data, strlen( data ), 0 ) < 0 )
	{
		printf( "client:\tsend error\n" );
		close( socket_FD );
		return -1;
	}

	printf( "client:\tsuccess\n" );

	close( socket_FD );

	return 0;
}
