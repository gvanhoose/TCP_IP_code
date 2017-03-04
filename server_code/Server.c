/**
 * Server.c
 *	author:		George H. VanHoose Sr., 2017
 *	prepared for:	CSE-453, University at Buffalo, State University of New York (SUNY)
 *			Professor Kris Schindler
 *			In pursuit of Adaptable Living Environment for Late-Stage ALS Patient
 *	version:	1
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "Server_Log.h"
#include "Arg_Builder.h"

#define BUFFER_LENGTH 64
#define NULL_CHAR '\0'
#define DEFAULT_PORT_NUMBER 5000
#define BACKLOG_MAX 1
#define TRUE 1
#define FALSE 0
#define SYSTEM_ERROR_NUMBER -1
#define LSOCKET_ERROR -3
#define LBIND_ERROR -2
#define QUIT_PROGRAM -4
#define LBAD_ARG_ERROR -5
#define QUIT_CHAR 'q'
#define LREAD_ERROR -6
#define LACCEPT_ERROR -7
#define LFORK_FAIL -8
#define LEXECV_FAIL -9

/**
 * A simple function that prints a message to console.
 * For use with debugging.
 */
void dbug()
{
	printf( "Reaching\n" );
}

// TODO:  Program needs to identify and flag invalid arguments.
//	most of the time this can be solved by merely requesting a retransmit
//	(especially since the original call (via the TCP_IP socket protocols)
//	is initiated by pressing a pre-set button
//	like a particular mouse-event
/**
 * spawnProcess
 *
 *	PARAMETERS:
 *		const	char*	arg
 *			The name of the program, and any arguments as may be neccessary, to be forked by this program.
 *
 *	RETURN:
 *		0x0 on success
 *		-1 on fork fail
 *
 *	NOTES:
 *		The child process does not return to this code once the syscall execv has been executed.
 *		There may be some merit to establishing a TCP_IP connection between the client and child process.
 *
 */
int spawnProcess( char*  arg)
{
	printf( "Reaching process spawn, with directive: \n\t%s\n(note:  No fork until proper functionality confirmed to this point!)\n", arg );

	int argLen = strlen( arg ), nargs = get_nargs( arg, argLen );

	char* args [ nargs ];

	int i;
	for( i = 0; i < nargs; i++ )	// have to initialize args & ensure it ready to go
	{
		args[i] = malloc( argLen + 1 );
		memset( args[i], '\0', argLen + 1 );
	}

	build_args( args, arg, argLen, nargs );

/*
 * commented-out for testing only!
 *
	pid_t currentPID = getpid();

	pid_t returnedPID = fork();

	if( returnedPID  < 0 )
	{
		printf( "Fork fail" );
		return( LFORK_FAIL );
	}
	if( returnedPID == 0 )
	{
		wait();		// may not need to wait, but we should take care to avoid the zombie apocalypse.
		return 0;	// parent process
	}

// program only proceeds to this area IF returned_PID is positive and nonzero

	pid_t childPID = getpid();

	execv( args[0], args );

	log_error( "execv failed while attempting to execute the following:" );

	char* errPrint = ("execv failed while attempting to execute:  %s", arg);

	log_error( errPrint );

	exit( 0 );
 *
 */
}

/**
 * openSocketAndBind
 *
 *	PARAMETERS:
 *		struct sockaddr_in sockAddr
 *			The internet socket address struct, which is laid out as follows:

struct sockaddr_in
{
	short		sin_family;	// the type of connection.  Commonly AF_INET for IPv4 connections (IPv4 uses the nnn.nnn.nnn.nnn IP address format; IPv6 uses the xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx address format)
	unsigned short	sin_port;	// the port number to be used for communication (typically htons(n) should be used to provide the argument for this variable, rather than entering the port number directly
	struct in_addr	sin_addr;	// a struct which serves as a wrapper for the IP address
	char		sin_zero[8];	// unknown / research more / not used here
};

 * and:

struct in_addr
{
	unsigned long	s_addr;		// the IP address.  htonl(n) should be used to construct this appropriately
};

 *
 *	RETURN:
 *		On success:  The file descriptor for the socket which was open and bound by this function
 *		On error:
 *			LSOCKET_ERROR	if the socket(...) call could not be executed properly
 *			LBIND_ERROR	if the bind(...) call could not be executed properly
 *
 *	NOTE:
 *		The caller need not worry about setting any values to sockAddr.
 *		This function initializes the entire struct to the relevant values, and sets unused variables to NULL
 *		This function uses the macro DEFAULT_PORT_NUMBER prescribed above.  This may be changed at the programmer's whim or fancy.
 *		Care must be taken to ensure the client has the correct port number when setting up the connection.
 */
int openSocketAndBind(struct sockaddr_in sockAddr)
{

	logEvent( "new call to \"openSocketAndBind\".  Result:" );

	int listenerFD;

	if( (listenerFD = socket( AF_INET, SOCK_STREAM, 0 )) == SYSTEM_ERROR_NUMBER )
	{
		perror( "socket error" );
		logAll( "socket error" );
		return LSOCKET_ERROR;
	}

	// zero out sockAddr and then set its values.
	memset( (char*) &sockAddr, NULL_CHAR, sizeof( sockAddr ) );
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	sockAddr.sin_port = htons( DEFAULT_PORT_NUMBER );

	if( bind( listenerFD, (struct sockaddr*) &sockAddr, sizeof( sockAddr ) ) == SYSTEM_ERROR_NUMBER )
	{
		perror( "bind error" );
		logAll( "bind error" );
		return LBIND_ERROR;
	}

	logEvent( "success" );
	return listenerFD;

}

/**
 * receiveMessage
 *
 *	PARAMETERS:
 *		int	listenerFD	The file descriptor of the socket through which communication takes place.
 *		char*	target		The string into which the message shall be copied from the socket
 *
 *	RETURN:
 *		0 on success
 *		LACCEPT_ERROR	The call to accept(...) was not executed properly
 *		LREAD_ERROR	The call to read(...) was not executed properly
 *		QUIT_PROGRAM	This is not an error, but a signal was received from the client that the entire server program needs to be closed.
 *
 *	NOTES:
 *		The program waits at the accept(...) call for a connection from a client.
 *		The call to listen(...) prepares the program for a number of connections, as defined by BACKLOG_MAX.  This may be changed, but for now it is set to 1
 *		Erroneous return from this function does not doom the program.  It simply means that a particular connection was unsuccessful.  Client will have to try again.  Investigation required if this becomes a repeat problem.
 *
 */
int receiveMessage( int listenerFD, char* target )
{

	listen( listenerFD, BACKLOG_MAX );

	int clientFD;

	printf( "Awaiting message.\n" );

	if( (clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL )) < 0 )
	{
		return LACCEPT_ERROR;
	}

	printf( "Message received!\n" );

	// this may require a loop to capture all of the message.
	int bytesRead;
	if( (bytesRead = read( clientFD, target, BUFFER_LENGTH - 1 )) < 0 )
	{
		printf( "read error\n" );
		return LREAD_ERROR;
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
/**
 *
At this point:
	openSocketAndBind(...) opens a socket on the RPi, and binds it
	if FALSE
		if the function returns a negative number, then an error has occurred.
		handling these potential errors will need to be much more robust
	on TRUE
		listenerFD will contain a file descriptor detailing where to find the socket
		the socket will also be bound
 *
 */
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

/**
 *
receiveMessage will wait until a client establishes a connection
	on FALSE
		program will terminate safely.
	on TRUE
		instruction will contain the name of a program to be executed
		instruction will also contain any ancillary arguments for such execution
		call to spawnProcess(...) will create a new process
		this new process will execute the program described by instruction
 *
 */

/*
 * commented-out while testing arg-buidling functionality in spawnProcess
 */
	int recReturnValue, keepGoing = TRUE;

	while( keepGoing )
	{
		while( (recReturnValue = receiveMessage( listenerFD, instruction )) >= 0 )
		{
			spawnProcess( instruction );
		}

		switch( recReturnValue )
		{
			case LACCEPT_ERROR:
			{
				// TODO: perror(); printf(); etc. as may be neccessary
				break;
			}
			case LREAD_ERROR:
			{
				// TODO: same as LACCEPT_ERROR
				break;
			}
			case QUIT_PROGRAM:
			{
				keepGoing = FALSE;
				break;
			}
			default:
			{
				break;
			}
		}

	}
 /*
 */
	return 0;

}
