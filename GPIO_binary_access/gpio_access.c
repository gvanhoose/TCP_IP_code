#include "gpio.h"

int buildPinRequests( int n_requests, char* argv [], struct Pin_Request pinRequests [] )
{
	if( n_requests < MIN_ARGS )
	{
//		log_error( "too few arguments" );
		return ERROR;
	}

	if( n_requests > MAX_PIN_COUNT )
	{
//		log_error( "Pin count is too high" );
		return ERROR;
	}

	int i;
	for( i = 0; i < n_requests; i++ )
	{

		int curLength = strlen( argv[ i + FIRST_REQUEST_INDEX ] ) + 1;

		char* current = argv[ i + FIRST_REQUEST_INDEX ];
		char* rawPinNumber;
		char* rawDirection;
		char* rawWriteValue;

		int pinNumber, pinValue;

		if( (rawPinNumber = strtok( current, PIN_NUMBER_DEMARKER )) == NULL )
		{
//			logError( "Invalid argument, missing pin number or incorrectly formatted" );
			printf( "Invalid argument, missing pin number or incorrectly formatted\n" );
			return PIN_REQUEST_ERROR( i );
		}

		if( (rawDirection = strtok( NULL, PIN_DIRECTION_DEMARKER )) == NULL )
		{
//			logError( "Invalid argument, missing pin direction or incorrectly formatted" );
			printf( "Invalid argument, missing pin direction or incorrectly formatted\n" );
			return PIN_REQUEST_ERROR( i );
		}

		rawWriteValue = strtok( NULL, PIN_TO_WRITE_DEMARKER );

		if(
			( strcmp( OUT, rawDirection ) == 0 && rawWriteValue == NULL )
			||
			( strcmp( IN, rawDirection  ) == 0 && rawWriteValue != NULL )
		  )
		{
//			logError( "Write value specified with request to Read, or no value specified with request to Write" );
			return PIN_REQUEST_ERROR( i );
		}

		// pin number must undergo a series of tests to ensure it reprents a valid pin
		if( (pinNumber = atoi( rawPinNumber )) == 0 )
		{
//			logError( "Invalid Pin Number" );
			return PIN_REQUEST_ERROR( i );
		}

		if( pinNumber < MIN_PIN_NUMBER || pinNumber > MAX_PIN_NUMBER )
		{
//			log_error( "Invalid pin number, out of range" );
			return PIN_REQUEST_ERROR( i );
		}

		// pin number has been verified to represent a valid pin
		pinRequests[ i ].pin_number = pinNumber;

		// building a request for input is different from building a request for output
		// therefore, the program branches based on the value of rawDirection
		if( strcmp( IN, rawDirection) == 0 )
		{
			if( rawWriteValue != NULL )
			{
//				log_error( "value specified on read request" );
				return PIN_REQUEST_ERROR( i );
			}

			pinRequests[ i ].dir = INPUT;
			pinRequests[ i ].value = UNREAD;

			continue;
		}

		else if( strcmp( OUT, rawDirection ) == 0 )
		{
			if( rawWriteValue == NULL )
			{
//				log_error( "value not specified on write request" );
				printf( "write value not given for pin output request\n" );
				return PIN_REQUEST_ERROR( i );
			}

			pinRequests[ i ].dir = OUTPUT;
			pinRequests[ i ].value = ( strcmp( HIGH_STR, rawWriteValue ) == 0 )? ACTIVE_HIGH : ( strcmp( LOW_STR, rawWriteValue ) == 0 )? ACTIVE_LOW : -1;

			if( pinRequests[ i ].value == -1 )
			{
//				log_error( "value not correctly specified on write" );
				printf( "write value for pin was not set correctly\n" );
				return PIN_REQUEST_ERROR( i );
			}

			continue;
		}
		else
		{
//			log_error( "pin direction not accurately specified" );
			printf( "pin direction not accurately specified\n" );
			return PIN_REQUEST_ERROR( i );
		}
	}

	return SUCCESS;
}

int executeRequests( int n_requests, struct Pin_Request requests [] )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[ i ];

		// first, make sure current.dir has a valid setting (INPUT or OUTPUT)
		if( current.dir != INPUT && current.dir != OUTPUT )
		{
//			logError( "pin direction not specified" );
			printf( "direction not specified\n" );
			return PIN_REQUEST_ERROR( i );
		}

		// assume they come in exported
		char dirPath [ BUFFER_LENGTH ];		// stream to /sys/class/gpio/gpio#/direction
		char rwPath [ BUFFER_LENGTH ];		// stream to /sys/class/gpio/gpio#/value, which can be read from or written to

		// zero out the strings to ensure they have a reachable null-terminator
		memset( dirPath, '\0', BUFFER_LENGTH );
		memset( rwPath, '\0', BUFFER_LENGTH );

		// loads the direction or value path into the respective buffer
		int dPath_length = snprintf( dirPath, BUFFER_LENGTH, "/sys/class/gpio/gpio%d/direction", current.pin_number );
		int rwPath_length = snprintf( rwPath, BUFFER_LENGTH, "/sys/class/gpio/gpio%d/value", current.pin_number );

		FILE* dir_stream;	// this is the stream to which "in" or "out" is written, its path is the same whether the request is a read or write
		if( (dir_stream = fopen( dirPath, FILE_WRITE )) == NULL)
		{
			return DIR_OPEN_ERROR;
		}

		// getting a space ready for, and subsequently populating, the "in" or "out" string to be written to the dir_stream
		char dir_write [4];
		memset( dir_write, '\0', 4 );
		int dir_length = snprintf( dir_write, 4, "%s", (current.dir == INPUT)? IN : OUT );

		if( fwrite( dir_write, 1, dir_length, dir_stream ) < 0 )
		{
			fclose( dir_stream );
//			logError( "write unsuccessful" );
			printf( "Write unsuccessful\n" );
			return DIR_WRITE_ERROR;
		}

		int dClRetVal;
		if( (dClRetVal = fclose( dir_stream )) != EOF && dClRetVal != 0 )
		{
//			logError( "direction stream close error" );
			printf( "Direction stream close error.\n" );
			return DIR_CLOSE_ERROR;
		}

		FILE* rw_stream;	// this is the stream that handles writing or reading the actual pin
		char val_str [2];

		if( current.dir == OUTPUT )
		{
			// write current.value (cast into a string format) to the stream
			if( (rw_stream = fopen( rwPath, FILE_WRITE  )) == NULL )
			{
				return WR_OPEN_ERROR;
			}

			// can snprintf print directly to the stream?
			// it's at least worth a shot
//			snprintf( rw_stream, 2, "%d", current.value );
			// nope, it won't even compile!

			memset( val_str, '\0', 2 );
			snprintf( val_str, 2, "%d", current.value );

			if( fwrite( val_str, 1, 2, rw_stream ) < 0 )
			{
				return PIN_REQUEST_ERROR( i );
			}

		}

		else if( current.dir == INPUT )
		{
			if( (rw_stream = fopen( rwPath, FILE_READ )) == NULL )
			{
				return PIN_REQUEST_ERROR( i );
			}

			char val_str [2];
			memset( val_str, '\0', 2 );

			if( fread( val_str, 1, 2, rw_stream ) < 0 )
			{
//				logError( "could not read from stream" );
				printf( "Could not read from stream.\n" );
				return PIN_REQUEST_ERROR( i );
			}
			if( val_str[1] != '\0' )
			{
//				logError( "non-null-terminating char at end of val_string" );
				printf( "Warning:  Non-null-terminating character at the end of val_str after read.  Observe and investigate for erratic behavior\n" );
//				return PIN_REQUEST_ERROR( i );
			}
		}
		else
		{
//			logError( "Unknown pin request" );
			printf( "Unknown or undecipherable pin request encountered.\n" );
			return ERROR;
		}

		int clReturn;
		if( (clReturn = fclose( rw_stream ) != 0x0) || (clReturn !=  EOF))
		{
//			logError( "could not close rw_stream" );
			printf( "Could not close read-write stream.\n" );
			return PIN_REQUEST_ERROR( i );
		}
	}

}
