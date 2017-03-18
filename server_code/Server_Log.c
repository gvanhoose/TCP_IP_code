#include "Server_Log.c"

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

	
