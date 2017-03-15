#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define BUFFER_LENGTH 64
#define ERROR_VALUE -1
#define NULL_CHAR '\0'
#define DEFAULT_PORT_NUMBER 5000
#define BACKLOG_MAX 1
#define TRUE 1
#define FALSE 0
#define ACCEPT_FAIL_MAX 25
#define RECEIVE_FORMAT "%s\0"
#define CONFIRMATION "1"
#define SYSTEM_ERROR_NUMBER -1
#define LSOCKET_ERROR -1
#define LBIND_ERROR -2
#define QUIT_PROGRAM -4
#define QUIT_CHAR 'q'

void dbug()
{
	printf( "Reaching\n" );
}

// TODO:  Program needs to identify and flag invalid arguments.
//	most of the time this can be solved by merely requesting a retransmit
//	(especially since the original call (via the TCP_IP socket protocols)
//	is initiated by pressing a pre-set button
//	like a particular mouse-event
int spawnProcess( const char*  arg)
{
	pid_t currentPID = getpid();

	pid_t returnedPID = fork();

	if( returnedPID  < 0 )
	{
		printf( "Fork fail" );
		exit( -1 );
	}
	if( returnedPID == 0 )
	{
		return 0;
	}

	// program only proceeds to this area IF returned_PID is positive and nonzero

	pid_t childPID = getpid();

	char* execArgs [1];

	execv( execArgs[0], execArgs );

	exit( 0 );

}

int openSocketAndBind(struct sockaddr_in sockAddr)
{
	int listenerFD;

//dbug();

	if( (listenerFD = socket( AF_INET, SOCK_STREAM, 0 )) == SYSTEM_ERROR_NUMBER )
	{
//dbug();

//		perror( "socket error" );
		return LSOCKET_ERROR;
	}

//	struct sockaddr_in* p_sockAddr = &sockAddr;

//dbug();
	memset( (char*) &sockAddr, NULL_CHAR, sizeof( sockAddr ) );	// \_ both cause a bug.
//	bzero( (char*) sockAddr, sizeof( sockAddr ) );		// /

//printf( "%d\n", sizeof( (char*) sockAddr ) );
//dbug();
	sockAddr.sin_family = AF_INET;
//dbug();
	sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
//dbug();
	sockAddr.sin_port = htons( DEFAULT_PORT_NUMBER );
//dbug();

	if( bind( listenerFD, (struct sockaddr*) &sockAddr, sizeof( sockAddr ) ) == SYSTEM_ERROR_NUMBER )
	{
//dbug();
//		perror( "bind error" );
		return LBIND_ERROR;
	}

//dbug();
	return listenerFD;

}

int receiveMessage( int listenerFD, char* target )
{

	listen( listenerFD, BACKLOG_MAX );

	int clientFD;

//	char receivedInstructions[ BUFFER_LENGTH ];	// passed in by arg "target"

//dbug();	// this executes on run
	clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL );
//dbug();	// this does not execute on run.  One can only assume it's waiting...
//	memset( target, NULL_CHAR, BUFFER_LENGTH );

// experiment follows:
//	it compiles just fine!
	read( listenerFD, target, sizeof( target ) - 1 );
//	snprintf( target, BUFFER_LENGTH, RECEIVE_FORMAT );	// temporarily commented-out

	if( target[0] == QUIT_CHAR && strlen( target ) == 1 )
	{
		close( clientFD );

		return QUIT_PROGRAM;
	}

//	write( clientFD, target, BUFFER_LENGTH );	// this writes the directive back to the client.  Not really necessary, but nice to include for down-the-road reliability, but we won't worry about it now.

//	spawnProcess( target );	// this is handled in the caller

	return 0;
}

int main( int argc, char* argv[] )
{
//	int listenerFD;
//
//	// initialize listenerFD to the file descriptor
//	//	for this process's IPV4 socket
//	if( (listenerFD = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
//	{
//		perror( "socket error" );
//		exit( ERROR_VALUE );
//	}
//
//	struct sockaddr_in* p_sockAddr;
//
//	memset( p_sockAddr, NULL_CHAR, sizeof( p_sockAddr ) );
//
//	p_sockAddr->sin_family = AF_INET;
//	p_sockAddr->sin_addr.s_addr = htonl( INADDR_ANY );
//	p_sockAddr->sin_port = htons( DEFAULT_PORT_NUMBER );
//
//	if( bind( listenerFD, (struct sockaddr*) p_sockAddr, sizeof( p_sockAddr ) ) )
//	{
//		perror( "bind error" );
//		exit( ERROR_VALUE );
//	}
//
	struct sockaddr_in sockAddr;

//if( p_sockAddr == 0x0 )
//{
//dbug();
//}

	int listenerFD;

// At this point:
//	openSocketAndBind(...) opens a socket on the RPi, and binds it
// if FALSE
//	if the function returns a negative number, then an error has occurred.
//	handling these potential errors will need to be much more robust
// on TRUE
//	listenerFD will contain a file descriptor detailing where to find the socket
//	the socket will also be bound

//dbug();
//	memset( (char*) p_sockAddr, NULL_CHAR, sizeof( p_sockAddr ) );
//dbug();
	if( (listenerFD = openSocketAndBind( sockAddr )) < 0 )
	{
		if( listenerFD == LSOCKET_ERROR )
		{
			perror( "socket error" );
		}
		if( listenerFD == LBIND_ERROR );
		{
			perror( "bind error" );
		}
		exit( SYSTEM_ERROR_NUMBER );
	}

//	dbug();

	char instruction [BUFFER_LENGTH];

//
// receiveMessage will wait until a client establishes a connection
// on FALSE
//	program will terminate safely.
// on TRUE
//	instruction will contain the name of a program to be executed
//	instruction will also contain any ancillary arguments for such execution
//	call to spawnProcess(...) will create a new process
//	this new process will execute the program described by instruction

	while( receiveMessage( listenerFD, instruction ) >= 0 )
	{
		spawnProcess( instruction );
	}

	return 0;

//	receiveMessage( listenerFD );

//	listen( listenerFD, BACKLOG_MAX );	// sleeps the server program until a client wakes it up. (similar to a semaphore)
//
//	int clientFD;
//
//	char receivedInstructions [ BUFFER_LENGTH ];
//
//	while( TRUE )
//	{
//		unsigned int acceptFailCount = ACCEPT_FAIL_MAX;
//
////		if( clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL )
//		while( clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL ) )
////		while( acceptFailCount-- > 0 && (clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL)) )
//		{
//			printf( "server failed to accept client.\n\t%d attempts remaining.\n", acceptFailCount );
//			perror( "server failed to accept client." );
//		}
//		if( acceptFailCount == 0 )
//		{
//			continue;	// turns the program back so that it cannot falsely execute the following code.
//// not sure if it is wise to simply turn the program back to the top of the while loop...
//// The program should be put back to sleep.
//// The client program may time-out naturally.  not sure how that behavior affects this program.
//
//
//		}
//
//		memset( receivedInstructions, NULL_CHAR, BUFFER_LENGTH );
//
//		snprintf( receivedInstructions, BUFFER_LENGTH, "%s\0" );
//
//		write( clientFD, receivedInstructions, BUFFER_LENGTH );	// may need strlen( receivedInstructions), depending on how the string is laid out.  (don't want any security holes!)
//
//		close( clientFD );
//
//		spawnProcess( receivedInstructions );


/*
 * moved to unobtrusive helper fucntion
		// thus begins the procedure for calling the program
		//	specified by the TCP_IP message
		pid_t current_PID = getpid();

		pid_t returned_PID = fork();

		if( returned_PID < 0 )
		{
			printf( "fork fail\n" );
		}
		else
		{
			if( ret_PID == 0 )
			{
				pid_t childPID =  getpid();


			}
 *
 */

// either fork to a dedicated program that carries out the instructions
// or write to the GPIO directly from this position.
//
// for the sake of modularity and supporting a dynamic number of components,
// perhaps forking with the more elegant solution.
// even better:  Give the desired program its own thread. (no delays!)

//		sleep( 1 );
//
//	}
//
//	return 0;
}
