#include <stdio.h>
#include <string.h>

#define UNEXPORT_PATH		"/sys/class/gpio/unexport"
#define FILE_WRITE		"w"

int main( int argc, char* argv [] )
{

	FILE* unexport_stream = fopen( UNEXPORT_PATH, FILE_WRITE );

	fwrite( argv[1], 1, strlen( argv[1] ), unexport_stream );

	fclose( unexport_stream );

	return 0;
}
