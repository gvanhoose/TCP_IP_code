#include "Server_Log.h"

FILE* openFile( const char* filename, const char* privileges )
{
	FILE* p_file;

	if( (p_file = fopen( filename, privileges )) == NULL )
	{
		perror( "file open error" );
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
	FILE* p_logFile;

	if( (p_logFile = openLogFile()) == NULL )
	{
		perror( "log file open error" );
		return OPEN_ERROR;
	}

	int printRetVal, closeRetVal;

	char* newLine = "\n";

	if( (printRetVal = fprintf( p_logFile, newLine )) < 0 )
	{
		perror( "newline character failed to print to log file" );
	}

	if( (printRetVal = fprintf( p_logFile, getTimestamp() )) < 0 )
	{
		perror( "timestamp failed to print to log file" );
	}

	if( (printRetVal = fprintf( p_logFile, eventDescr )) < 0 )
	{
		perror( "event description failed to print to log file" );
	}

	if( (closeRetVal = fclose( p_logFile )) != 0 )
	{
		perror( "log file failed to close properly" );
		return (printRetVal < 0)? WRITE_CLOSE_ERROR : CLOSE_ERROR;
	}

	return (printRetVal < 0)? WRITE_ERROR : 0;

}


int logError( const char* errDescr )
{
	FILE* p_errFile;

	if( (p_errFile = openErrFile()) == NULL )
	{
		perror( "err file open error" );
		return OPEN_ERROR;
	}

	int printRetVal, closeRetVal;

	char* newLine = "\n";

	if( (printRetVal = fprintf( p_errFile, newLine )) < 0 )
	{
		perror( "newline character failed to print to error log file" );
	}

	if( (printRetVal = fprintf( p_errFile, getTimestamp() )) < 0 )
	{
		perror( "timestamp failed to print to error log file" );
	}

	if( (printRetVal = fprintf( p_errFile, errDescr )) < 0 )
	{
		perror( "event description failed to print to error log file" );
	}

	if( (closeRetVal = fclose( p_errFile )) != 0 )
	{
		perror( "error log file failed to close properly" );
		return (printRetVal < 0)? WRITE_CLOSE_ERROR : CLOSE_ERROR;
	}

	return (printRetVal < 0)? WRITE_ERROR : 0;

}

int logAll( const char* eventDescr )
{
	int logRet, errRet;

	logRet = logEvent( eventDescr );
	errRet = logError( eventDescr );

	return (logRet == 0 && errRet == 0)? 0 : (logRet != 0)? logRet : errRet;
}
