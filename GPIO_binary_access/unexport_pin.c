#include <stdio.h>
#include <string.h>
#include "exUnex.h"

#ifndef SUCCESS
#define SUCCESS	0
#endif

int main( int argc, char* argv [] )
{
	FILE* unexport_stream;

	if( (unexport_stream = fopen( UNEXPORT_PATH, FILE_WRITE )) == NULL )
	{
		return UNEXP_PATH_ERROR;
//		exit( UNEXP_PATH_ERROR );
	}

	if( fwrite( argv[1], 1, strlen( argv[1] ), unexport_stream ) < 0 )
	{
		return UNEXP_WRITE_ERROR;
//		exit( UNEXP_WRITE_ERROR );
	}

	fclose( unexport_stream );

	return SUCCESS;
//	exit( SUCCESS );
}
