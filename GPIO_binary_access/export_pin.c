#include <stdio.h>
#include <string.h>
#include "exUnex.h"

#ifndef SUCCESS
#define SUCCESS	0
#endif

int main( int argc, char* argv [] )
{
	FILE* export_stream;

	if( (export_stream = fopen( EXPORT_PATH, FILE_WRITE )) == NULL )
	{
		return EXP_PATH_ERROR;
//		exit( EXP_PATH_ERROR );
	}

	if( fwrite( argv[1], 1, strlen( argv[1] ), export_stream ) < 0 )
	{
		return EXP_WRITE_ERROR;
//		exit( EXP_WRITE_ERROR );
	}

	fclose( export_stream );

	return SUCCESS;
//	exit( SUCCESS );
}
