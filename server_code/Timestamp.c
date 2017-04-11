#include "Timestamp.h"

char* getTimestamp()
{
	time_t rawTime;

	struct tm* nstamp;

	time( &rawTime );

	nstamp = localtime( &rawTime );

	char* timestamp = asctime( nstamp );

	timestamp[ strlen(timestamp) - 1 ] = '=';	// repurposes the null-term in the string with something that won't force a line break, and which will serve a more intuitive purpose!

	return timestamp;
}
