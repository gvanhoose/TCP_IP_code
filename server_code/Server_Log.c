#include "Server_Log.h"

FILE* openFile( const char* filename, const char* fileAccess )
{
	FILE* p_file;

	if( (p_file = fopen( filename, fileAccess )) == NULL )
	{
		perror( "file open error" );
		return p_file;
	}

	return p_file;
}
FILE* openLogFile()
{
	return openFile( LOG_FILENAME, APPEND_ONLY );
}
FILE* openErrFile()
{
	return openFile( ERR_FILENAME, APPEND_ONLY );
}

int logEvent( const char* eventDescr )
{
	FILE* p_LogFile;

	if( (p_LogFile = openLogFile()) == NULL )
	{
		perror( ("Attempt to open \"%s\" failed.", LOG_FILENAME) );
		return OPEN_ERROR;
	}

	int printReturn, closeReturn;
	
	int timestamp [6];

	char tStamp [13];
	
	int demarkerIndex, digitIndex;
	
	if( setTimeStamp( timestamp ) != 0 )
	{
		int i;
		for( i = 0; i < 6; i++ )
		{
			timestamp[i] = -1;
		}
	}
	
	char tStamp [] = { ("%d:%d:%d %d/%d/%d ", timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4], timestamp[5]) };

	
	printf( "Timestamp: %s\n", tStamp );
	
	if( (printReturn = fprintf( p_LogFile, tStamp )) < 0 )
	{
		perror( "Timestamp failed to print" );
	}

	if( (printReturn = fprintf( p_LogFile, eventDescr )) < 0 )
	{
		if( (closeReturn = fclose( p_LogFile )) != 0 )
		{
			perror( ("Attempt to print (append) \"%s\" failed.  Attempt to close \"%s\" failed.", LOG_FILENAME, LOG_FILENAME) );
			return closeReturn;
		}

		perror( ("Attempt to print (append) \"%s\" failed. Closed successfully.", LOG_FILENAME) );
		return printReturn;
	}

	if( (closeReturn = fclose( p_LogFile )) != 0 )
	{
		perror( ("Attempt to close \"%s\" failed.", LOG_FILENAME) );
	}

	return closeReturn;
}

int logError( const char* eventDescr )
{
	FILE* p_ErrFile;

	if( (p_ErrFile = openLogFile()) == NULL )
	{
		perror( ("Attempt to open \"%s\" failed.", ERR_FILENAME) );
		return OPEN_ERROR;
	}

	int printReturn, closeReturn;

	if( (printReturn = fprintf( p_ErrFile, eventDescr )) < 0 )
	{
		if( (closeReturn = fclose( p_ErrFile )) != 0 )
		{
			perror( ("Attempt to print (append) \"%s\" failed.  Attempt to close \"%s\" failed.", ERR_FILENAME, ERR_FILENAME) );
			return closeReturn;
		}

		perror( ("Attempt to print (append) \"%s\" failed. Closed successfully.", ERR_FILENAME) );
		return printReturn;
	}

	if( (closeReturn = fclose( p_ErrFile )) != 0 )
	{
		perror( ("Attempt to close \"%s\" failed.", ERR_FILENAME) );
	}

	return closeReturn;
}
