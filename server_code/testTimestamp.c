#include "Timestamp.h"

int main()
{
	int nstamp [6];
	
	setTimeStamp( nstamp );
	
	int i;
	for( i = 0; i < 6; i++ )
	{
		printf( "nstamp[ %d ]\n", nstamp[i] );
	}
	
	char timestamp [] = { ( "%d:%d:%d %d/%d/%d\0", nstamp[0], nstamp[1], nstamp[2], nstamp[3], nstamp[4], nstamp[5] ) };
	
	printf( "timestamp:  \"%s\"\n", timestamp );
	
	return 0;
}
