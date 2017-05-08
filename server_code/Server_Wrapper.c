/**
 * The purpose of this program is to provide total protection for the server against any kind of crash.
 */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t retPid;

	int rawExitReturn = 0x0, exitSuccess;

	while( 1 )
	{

		printf( "Preparing to fork\n" );

		retPid = fork();

		printf( "Fork returned with pid %d\n", retPid );

		if( retPid < 0 )
		{
			printf( "Fork fail\n" );
			return 0;
		}

		if( retPid != 0 )	// parent code
		{
			printf( "Parent:\tAbout to wait\n" );

			wait( &rawExitReturn );

			exitSuccess = ( WIFEXITED( rawExitReturn ) );	// updates waitStatus with an exit code

			printf( "Parent:\twait finished, exit analysis:\n\traw exit:\t%d\n\texit code:\t%d\n", rawExitReturn, exitSuccess );

			if( exitSuccess )
			{
				printf( "Safe server termination\n" );

				break;
			}

			printf( "Preparing to sleep while Linux releases the socket for reuse\n" );

			sleep( 120 );
			continue;
		}

		// child code

		char* cmd [1];
		cmd[0] = "/home/pi/On_Startup/server";

		printf( "Child:\tAbout to execute\n" );

		execv( cmd[0], cmd );

		printf( "Child:\tExecute fail\n" );

		exit( EXIT_FAILURE );
	}

	printf( "server %s terminated\n", (exitSuccess)? "successfully" : "unsuccessfully" );

	return 0;
}
