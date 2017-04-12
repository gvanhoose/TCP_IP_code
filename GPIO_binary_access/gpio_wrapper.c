#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

void auditPinRequests( const struct Pin_Request* requests, int n_requests )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		printf( "Pin %d\n\t\"%s\"\n\t\"%d\"\n", requests[i].pin_number, requests[i].direction, requests[i].value );
	}
}

int main( int argc, char* argv [] )
{
//	printf( "Reaching:  argc = %d\n", argc );
//	int n_requests = atoi( argv [1] );	// clean this up later, check for errors
//
//	if( n_requests == 0 || n_requests > MAX_PIN_COUNT )
//	{
//		exit( PIN_COUNT_ERROR );
//	}

	int n_requests = argc - 1;	// replaces LOTS of code, and eliminates an arg from the commandline to launch this program!

	struct Pin_Request requests [ n_requests ];

	int buildReturnValue;

	printf( "about to build requests\n" );
	if( ( buildReturnValue = buildPinRequests( n_requests, argv, requests )) != SUCCESS )
	{
		printf( "failed to build pin requests\n" );
		// code to attempt to fix could go here, if you wanted...
		exit(buildReturnValue);
	}
	printf( "completed build requests\n" );

	auditPinRequests( requests, n_requests );

	exit( SUCCESS );
}
