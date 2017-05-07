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

#define BUFFER_LENGTH		128
#define DEFAULT_PORT_NUMBER	5000
#define BACKLOG_MAX		1	// this is the maximum number of connections that the server can handle at any one time

#define QUIT_CHAR	'q'
#define NULL_CHAR	'\0'

#define TRUE		1
#define FALSE		0

#define SUCCESS			0
#define SYSTEM_ERROR_NUMBER	-1
#define LSOCKET_ERROR		-3
#define LBIND_ERROR		-2
#define QUIT_PROGRAM		-4
#define LBAD_ARG_ERROR		-5
#define LREAD_ERROR		-6
#define LACCEPT_ERROR		-7
#define LFORK_FAIL		-8
#define LEXECV_FAIL		-9
#define LWAIT_FAIL		-10
#define LLISTEN_ERROR		-11

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
	int argLen = strlen( arg ), nargs = get_nargs( arg, argLen );

	char* args [ nargs ];

	int i;
	for( i = 0; i < nargs; i++ )
	{
		args[i] = malloc( argLen + 1 );		// guarantees each string in args will be able to contain any argument
		memset( args[i], '\0', argLen + 1 );	// good form to initialize the whole string to the null-term
	}

	// translates the single string sent by the client to the "char* argv []" format required by execv()
	build_args( args, arg, argLen, nargs );

	pid_t returnedPID = fork();	// Remember:  When returning from fork(), the child has 0x0 for this value, parent has the child pid for this value

	// fork fail
	if( returnedPID  < 0 )
	{
		logAll( "Fork failed" );
		return LFORK_FAIL;
	}

	// parent code
	if( returnedPID > 0 )
	{
		// parent process execution
		if( wait() != 0)		// may not need to wait, but we should take care to avoid the zombie apocalypse.
		{
			logAll( "Wait failed" );
			return LWAIT_FAIL;
		}
		return SUCCESS;
	}

	// child code

	pid_t childPID = getpid();

	execv( args[0], args );

	// if the following lines executes, then execv failed

	char* errPrint = ("execv failed while attempting to execute:  %s", arg);

	logError( errPrint );

	exit( LEXECV_FAIL );
}

/**
 * openSocketAndBind
 *
 *	PARAMETERS:
 *		struct sockaddr_in sockAddr
 *			The internet socket address struct
 *			Contents:
 *				type of connection
 *				port number
 *				ip address struct ( struct in_addr, which contains single variable for the IP address )
 *
 *	RETURN:
 *		On success:  The file descriptor for the socket which was open, bound, and on which listen is called
 *		On error:
 *			LSOCKET_ERROR	if the socket() call could not be executed properly
 *			LBIND_ERROR	if the bind() call could not be executed properly
 *			LLISTEN_ERROR	if the listen() call could not be executed properly
 *
 *	NOTE:
 *		The caller need not worry about setting any values to sockAddr.
 *		This function initializes the entire struct to the relevant values, and sets unused variables to NULL
 *		This function uses the macro DEFAULT_PORT_NUMBER prescribed above.  This may be changed at the programmer's whim or fancy.
 *		Care must be taken to ensure the client has the correct port number when setting up the connection.
 */
int openSocketAndBind(struct sockaddr_in sockAddr)
{

	int listenerFD;

	// first, get the file descriptor of the desired socket
	if( (listenerFD = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
	{
		return LSOCKET_ERROR;
	}

	// zero out sockAddr and then set its values.
	memset( (char*) &sockAddr, NULL_CHAR, sizeof( sockAddr ) );
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	sockAddr.sin_port = htons( DEFAULT_PORT_NUMBER );

	// binding the socket couples the socket (pointed to by listenerFD) to this pid
	if( bind( listenerFD, (struct sockaddr*) &sockAddr, sizeof( sockAddr ) ) < 0 )
	{
		return LBIND_ERROR;
	}

	// essentially, this configures the socket for input on the socket (see "man 2 listen")
	if( listen( listenerFD, BACKLOG_MAX ) < 0 )
	{
		return LLISTEN_ERROR;
	}

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
 *		LACCEPT_ERROR	The call to accept() was not executed properly
 *		LREAD_ERROR	The call to read() was not executed properly
 *
 *	NOTES:
 *		The program waits at the accept() call for a connection from a client.
 *		Erroneous return from this function does not doom the program.  It simply means that a particular connection was unsuccessful.  Client will have to try again.  Investigation required if this becomes a repeat problem.
 *
 */
int receiveMessage( int listenerFD, char* target )
{
	int clientFD;

	// program waits here until client connects
	if( (clientFD = accept( listenerFD, (struct sockaddr*) NULL, NULL )) < 0 )
	{
		return LACCEPT_ERROR;
	}

	// retrieves the data sent by the client
	int bytesRead;
	if( (bytesRead = read( clientFD, target, BUFFER_LENGTH - 1 )) < 0 )
	{
		return LREAD_ERROR;
	}

	// place the null-terminator at the end of the data
	target[ bytesRead ] = NULL_CHAR;

	return SUCCESS;
}

int main( int argc, char* argv[] )
{
	logAll( "\n=======================================================\n" );

	// Good idea:  Record the pid of this program
	char pid_str [35];
	memset( pid_str, '\0', 35 );

	snprintf( pid_str, 35, "Server PID:  %d", getpid() );

	logEvent( pid_str );


	struct sockaddr_in sockAddr;

	int listenerFD;

	// openSocketAndBind does four things:
	//	opens the socket
	//	binds the socket to this program
	//	configures this program to listen for input on that socket
	//	returns
	//		SUCCESS:	The file descriptor for the socket
	//		ERROR:		An error code
	// IMPORTANT:
	//	If openSocketAndBind returns an error code, it is very possible that this program need only wait for the kernel to release the socket
	//	therefore, the program is given infinite attempts at this functionality, since nothing else can happen anyway!
	//	typical wait should be about one minute
	//
//	if( (listenerFD = openSocketAndBind( sockAddr )) < 0 )
	while( (listenerFD = openSocketAndBind( sockAddr)) < 0 )
	{
		if( listenerFD == LSOCKET_ERROR )
		{
			logError( "Open socket error" );
		}
		if( listenerFD == LBIND_ERROR );
		{
			logError( "Bind socket error" );
		}

		sleep( 20 );			// sleeping for 20 seconds may give the OS enough time to clear out the error
//		return SYSTEM_ERROR_NUMBER;	// remains commented-out while this is part of a while loop
//		exit( SYSTEM_ERROR_NUMBER );
	}
	logEvent( "Socket ready" );

	char instruction [BUFFER_LENGTH];	// the buffer which will hold the data sent by the client

	int recReturnValue;

	listen( listenerFD, BACKLOG_MAX );	// moved from inside "receiveMessage(...)", since multiple calls might be causing server to crash

	while( TRUE )
	{
		// receiveMessage waits for a connection, then populates the buffer named "instruction"
		if( (recReturnValue = receiveMessage( listenerFD, instruction )) >= 0 )
		{

			int instrLen = strlen( instruction );

			instruction[ instrLen -= 2 ] = '\0';

			// if the client send "q", this safely terminates the server such that it can be resumed later.
			if( (instrLen = strlen( instruction )) == 1 && *instruction == 'q' )
			{
				break;
			}

			// forks a process which uses the data from client to execute a program
			spawnProcess( instruction );

			logEvent( "spawn complete\n===================================================\n" );
//			sleep( 2 );	// sleeping this thread avoids overrunning the code	// may be causing problems
			continue;
		}

		switch( recReturnValue )
		{
			case LACCEPT_ERROR:
			{
				logError( "Client connection not properly accepted" );
				continue;
			}
			case LREAD_ERROR:
			{
				logError( "Client data could not be read" );
				continue;
			}
			case QUIT_PROGRAM:
			{
				logEvent( "Client transmitted server termination" );
				break;
			}
			default:
			{
				logAll( "Unknown return value from receiveMessage()" );
				continue;
			}
		}

	}

	// safely closes the socket such that subsequent executions of this program may use the same port
	close( listenerFD );

	return SUCCESS;

}
