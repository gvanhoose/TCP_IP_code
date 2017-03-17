#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define APPEND_ONLY       "a"
#define LOG_FILENAME      "Log_File.txt"
#define ERR_FILENAME      "Error_File.txt"
#define PROGRAM_ERROR     -1
#define OPEN_ERROR        -2
#define WRITE_ERROR       -3
#define WRITE_CLOSE_ERROR -4
#define CLOSE_ERROR       -5

FILE* openFile( const char*, const char* );
FILE* openLogFile();
FILE* openErrFile();

int logEvent( const char* );

int logError( const char* );
