#include "gpio.h"

#define SINGLE_REQUEST	1

#define LIGHT_ON	"4,out=HIGH"

int main()
{
	struct Pin_Request request [ SINGLE_REQUEST ];

	int buildReturnValue;

	char* args [2];

	char blank [] = "";
	char live_arg [] = LIGHT_ON;

	args[0] = blank;	// leave this blank, required for compatibility with call to buildPinRequests
	args[1] = live_arg;

	if( (buildReturnValue = buildPinRequests( SINGLE_REQUEST, args, request )) != SUCCESS )
	{
		exit( buildReturnValue );
	}

	exit( executeRequests( SINGLE_REQUEST, request ) );

}
