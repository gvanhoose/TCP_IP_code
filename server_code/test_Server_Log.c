#include <stdio.h>
#include <stdlib.h>
#include "Server_Log.h"

int main()
{
	logEvent( "Hello World!" );

	logError( "execv failed to execute, Lamp was not toggled as such" );

	logAll( "Taxation is theft" );

	return 0;
}
