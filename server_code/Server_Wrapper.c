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

		retPid = fork();

		if( retPid < 0 )	// fork fail
		{
			return 0;
		}

		if( retPid != 0 )	// parent code
		{

//			printf( "Server running on PID %d\n", retPid );

			wait( &rawExitReturn );

//			if( signal( SIGUSR1, NULL )	// have child send signal on successful termination!

			exitSuccess = ( WIFEXITED( rawExitReturn ) );	// updates waitStatus with an exit code

			if( exitSuccess )
			{
				break;
			}

			sleep( 120 );
			continue;
		}



		// child code

		char* cmd [2];	// testing with server in background
//		char* cmd [1];
//		cmd[0] = "/home/pi/On_Startup/server";			// path for testing on George's home pi
		cmd[0] = "/home/pi/Desktop/On_Startup/server";		// path for actual device in Tim's room
		cmd[1] = "&";

		execv( cmd[0], cmd );

		exit( EXIT_FAILURE );
	}

	return 0;
}
