#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define EXPORT_PATH		"/sys/class/gpio/export"
#define FILE_WRITE		"w"

int main( int argc, char* argv [] )
{

	FILE* export_stream = fopen( EXPORT_PATH, FILE_WRITE );

	fwrite( argv[1], 1, strlen( argv[1] ), export_stream );

	fclose( export_stream );

	return 0;
}
