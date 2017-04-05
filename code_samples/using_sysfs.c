#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN	0
#define OUT	1

#define HIGH	1
#define LOW	0

#define P_IN	24	/* p1-18 */
#define P_OUT	4	/* p1-07 */

#define BUFFER_MAX	3

static int GPIO_Export( int pin )
{
	char buffer [ BUFFER_MAX ];

	int bytes_written, file_descriptor;

	if( (file_descriptor = open( "/sys/class/gpio/export", O_WRONLY )) < 0 )
	{
		fprintf( stderr, "Failed to open export for writing!" );
		return -1;
	}

	bytes_written = snprintf( buffer, BUFFER_MAX, "%d", pin );
	write( file_descriptor, buffer, bytes_written );

	close( file_descriptor );

	return 0;
}

static int GPIO_unexport( int pin )
{
	char buffer [ BUFFER_MAX ];

	int file_descriptor, bytes_written;

	if( (file_descriptor = open( "/sys/class/gpio/unexport", O_WRONLY )) < 0 )
	{
		fprintf( stderr, "Failed to open unexport for writing!" );
		return -1;
	}

	bytes_written = snprintf( buffer, BUFFER_MAX, "%d", pin );

	write( file_descriptor, buffer, bytes_written );

	close( file_descriptor );

	return 0;
}

#define DIRECTION_MAX	35
static int GPIO_direction( int pin, int direction )
{
	static const char s_directions_str [] = "in out";	// change ' ' to null term!

	char path [ DIRECTION_MAX ];

	int file_descriptor;

	snprintf( path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin )

	if( (file_descriptor = open( path, O_WRONLY )) < 0 )
	{
		fprintf( stderr, "Failed to open gpio direction for writing" );
		return -1;
	}

	if( write( file_descriptor, &s_directions_str[ (IN == dir)? 0:3 ], (IN == dir)? 0:3 ) < 0 )
	{
		fprintf( stderr, "Failed to set direction" );
		return -1;
	}

	close( file_descriptor );

	return 0;
}

#define VALUE_MAX	30
static int GPIO_read( int pin )
{
	char path [ VALUE_MAX ];
	char value_str [ 3 ];

	int file_descriptor;

	snprintf( path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin );

	if( (file_descriptor = open( path, O_RDONLY )) < 0 )
	{
		fprintf( stderr, "Failed to open GPIO value for reading" );
		return -1;
	}

	if( read( fd, value_str, 3 < 0 ) )
	{
		fprintf( stderr, "Failed to read value" );
		return -1;
	}

	close( file_descriptor );

	return atoi( value_str );
}

static int GPIO_write( int pin, int value )
{
	static const char s_values_str [] = "01";

	char path [ VALUE_MAX ];

	int file_descriptor;

	snprintf( path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin );

	if( (file_descriptor = open( path, O_WRONLY )) < 0 )
	{
		fprintf( stderr, "Failed to open gpio value for writing" );
		return -1;
	}
	if( write( file_descriptor, &s_values_str[ (value == LOW)? 0:1 ], 1 ) < 0 )
	{
		fprintf( stderr, "Failed to write value!" );
		return -1;
	}

	close( file_descriptor );

	return 0;
}

int main( int argc, char* argv [] )
{
	int repeat = 10;

	// Enable GPIO pins
	if( GPIO_export( P_OUT ) < 0 OR  GPIO_export( P_IN ) < 0 )
	{
		return 1;
	}

	// Set GPIO direction(s)
	if( GPIO_direction( P_OUT, OUT ) < 0 OR GPIO_direction( P_IN, IN ) < 0 )
	{
		return 2;
	}

	do
	{
		// Write GPIO value
		if( GPIO_write( P_OUT, repeat % 2 ) < 0 )
		{
			return 3;
		}

		// Read GPIO value
		printf( "I'm reading %d in GPIO %d", GPIO_read( P_IN ), P_IN );

		usleep( 500 * 1000 );
	} while( repeat-- );

	// Disable GPIO pins
	if( GPIO_unexport( P_OUT ) < 0 OR GPIO_unexport( P_IN ) < 0 )
	{
		return 4;
	}

	return 0;
}
