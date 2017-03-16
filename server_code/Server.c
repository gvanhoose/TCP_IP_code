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
#define LSOCKET_ERROR -3
#define LBIND_ERROR -2
#define QUIT_PROGRAM -4
#define QUIT_CHAR 'q'
#define READ_ERROR -5

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
printf( "Reaching process spawn, with directive: \n\t%s\n(note:  No fork until proper functionality confirmed to this point!)\n", arg );
/*
 * commented-out for testing only!
 *
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
 *
 */
}

int openSocketAndBind(struct sockaddr_in sockAddr)
{
	int listenerFD;

	if( (listenerFD = socket( AF_INET, SOCK_STREAM, 0 )) == SYSTEM_ERROR_NUMBER )
	{
//		perror( "socket error" );
		return LSOCKET_ERROR;
	}

	memset( (char*) &sockAddr, NULL_CHAR, sizeof( sockAddr ) );

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	sockAddr.sin_port = htons( DEFAULT_PORT_NUMBER );

	if( bind( listenerFD, (struct sockaddr*) &sockAddr, sizeof( sockAddr ) ) == SYSTEM_ERROR_NUMBER )
	{
//		perror( "bind error" );
		return LBIND_ERROR;
	}

	return listenerFD;

}

int receiveMessage( int listenerFD, char* target )
{

	listen( listenerFD, BACKLOG_MAX );

	int clientFD;

	clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL );


	// this may require a loop to capture all of the message.
	int bytesRead;
	if( (bytesRead = read( clientFD, target, BUFFER_LENGTH - 1 )) < 0 )
	{
		printf( "read error\n" );
		return READ_ERROR;
	}

	target[ bytesRead ] = NULL_CHAR;

// keep this block around, just in case...
////	int bytesRead = 0;
//
////	while( (bytesRead = read( clientFD, target, sizeof( target ) - 1)) > 0 )
////	{
//		totalBytesRead += iterBytesRead;

////		target[ bytesRead ] = NULL_CHAR;
//
//		if( fputs( target, stdout ) == EOF )
//		{
//			printf( "fputs error" );
//		}
////	}

	if( target[0] == QUIT_CHAR && strlen( target ) == 1 )
	{
		close( clientFD );

		return QUIT_PROGRAM;
	}

	return 0;
}

int main( int argc, char* argv[] )
{
	struct sockaddr_in sockAddr;

	int listenerFD;

// At this point:
//	openSocketAndBind(...) opens a socket on the RPi, and binds it
// if FALSE
//	if the function returns a negative number, then an error has occurred.
//	handling these potential errors will need to be much more robust
// on TRUE
//	listenerFD will contain a file descriptor detailing where to find the socket
//	the socket will also be bound
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

}
