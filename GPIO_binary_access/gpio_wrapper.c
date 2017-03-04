#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

void auditPinRequests( const struct Pin_Request* requests, int n_requests )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		printf( "Pin %d\n\t%s\n\t%d\n", requests[i].pin_number, requests[i].direction, requests[i].value );
	}
}

int main( int argc, char* argv [] )
{

	int n_requests = atoi( argv [1] );	// clean this up later, check for errors

	if( n_requests == 0 || n_requests > MAX_PIN_COUNT )
	{
		exit( PIN_COUNT_ERROR );
	}

	struct Pin_Request requests [n_requests];

	int buildReturnValue;

	if( buildReturnValue = (buildPinRequests( argc, argv, requests, n_requests )) != SUCCESS )
	{
		// code to attempt to fix could go here, if you wanted...
		exit(buildReturnValue);
	}

	auditPinRequests( requests, n_requests );

	exit( SUCCESS );
}
