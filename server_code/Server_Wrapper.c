/**
 * The purpose of this program is to provide total protection for the server against any kind of crash.
 */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

//#define REBOOT_PI_FILEPATH "/home/pi/On_Startup/

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

			printf( "Server running on PID %d\n", retPid );

			wait( &rawExitReturn );

			exitSuccess = ( WIFEXITED( rawExitReturn ) );	// updates waitStatus with an exit code

			if( exitSuccess )
			{
				break;
			}

//			printf( "Server quit unexpectedly.  Attempting to restart\n" );
			printf( "Server quit unexpectedly.  Rebooting pi\n" );

			system( "sudo shutdown -r now" );	// God help me if this malfunctions

			return 0;	// this should never execute, but the compiler wants it!

//			sleep( 120 );

//			printf( "Server should restart\n" );
//			continue;
		}



		// child code

//		char* cmd [2];	// testing with server in background
		char* cmd [1];
//		cmd[0] = "/home/pi/On_Startup/server";			// path for testing on George's home pi
		cmd[0] = "/home/pi/Desktop/On_Startup/server";		// path for actual device in Tim's room
//		cmd[1] = "&";
//		cmd[0] = "./server";

//		printf( "Child:\tAbout to execute new server on pid %d\n\tcommand:\t%s\n", getpid(), cmd[0] );

		execv( cmd[0], cmd );

		exit( EXIT_FAILURE );
	}

	return 0;
}
