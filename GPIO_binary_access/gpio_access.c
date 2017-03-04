#include "gpio.h"

int buildPinRequests( int argc, char* argv [], struct Pin_Request pinRequests [], int n_requests )
{
	if( argc < MIN_ARGS )
	{
//		log_error( "too few arguments" );
		return ERROR;
	}

	int pin_count;
	if( (pin_count = atoi( argv[ PIN_COUNT_INDEX ] )) == 0 )
	{
//		log_error( "No pin count provided" );
		return ERROR;
	}

	if( pin_count < 0 || pin_count > MAX_PIN_COUNT )
	{
//		log_error( "Pin count is too high" );
		return ERROR;
	}

//	struct Pin_Request pinRequests [ pin_count ];

	int i;
	for( i = 0; i < pin_count; i++ )
	{
		char* current = argv[ i + PIN_COUNT_INDEX + 1 ];
		char* rawPinNumber;
		char* rawDirection;
		char* rawWriteValue;

		int pinNumber, pinValue;

		if( (rawPinNumber = strtok( current, PIN_NUMBER_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin number or incorrectly formatted" );
			return ERROR;
		}

		if( (rawDirection = strtok( NULL, PIN_DIRECTION_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin direction or incorrectly formatted" );
			return ERROR;
		}

		rawWriteValue = strtok( NULL, PIN_TO_WRITE_DEMARKER );

		if(
			( strcmp( OUT, rawDirection ) == 0 && rawWriteValue == NULL )
			||
			( strcmp( IN, rawDirection  ) == 0 && rawWriteValue != NULL )
		  )
		{
//			log_error( "Write value specified with request to Read, or no value specified with request to Write" );
			return ERROR;
		}

		if( (pinNumber = atoi( rawPinNumber )) == 0 )
		{
//			log_error( "Invalid Pin Number" );
			return ERROR;
		}

		if( pinNumber < MIN_PIN_NUMBER || pinNumber > MAX_PIN_NUMBER )
		{
//			log_error( "Invalid pin number, out of range" );
			return ERROR;
		}

		pinRequests[ i ].pin_number = pinNumber;

		if( strcmp( IN, rawDirection) == 0 )
		{
			if( rawWriteValue != NULL )
			{
//				log_error( "value specified on read request" );
				return ERROR;
			}
			pinRequests[ i ].direction = rawDirection;
			pinRequests[ i ].value = UNREAD;

//			code to read pins goes here
//			basically, read from "/sys/class/gpio/gpio#/value" where # is pinNumber
//			don't forget to set pinRequests[ i ].value!

			continue;
		}

		else if( strcmp( OUT, rawDirection ) == 0 )
		{
			if( rawWriteValue == NULL )
			{
//				log_error( "value not specified on write request" );
				return ERROR;
			}
			pinRequests[ i ].direction = rawDirection;
			pinRequests[ i ].value = ( strcmp( HIGH_STR, rawWriteValue ) == 0 )? ACTIVE_HIGH : ( strcmp( LOW_STR, rawWriteValue ) == 0 )? ACTIVE_LOW : -1;

			if( pinRequests[ i ].value == -1 )
			{
//				log_error( "value not correctly specified on write" );
				return ERROR;
			}

			continue;
		}
		else
		{
//			log_error( "pin direction not accurately specified" );
			return ERROR;
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

		// assume they come in exported

		char dirPath [ BUFFER_LENGTH ];
		char rwPath [ BUFFER_LENGTH ];

		memset( dirPath, '\0', BUFFER_LENGTH );
		memset( rwPath, '\0', BUFFER_LENGTH );

		int dPath_length = snprintf( dirPath, BUFFER_LENGTH, "/sys/class/gpio/gpio%d/direction", current.pin_number );
		int rwPath_length = snprintf( rwPath, BUFFER_LENGTH, "/sys/class/gpio/gpio%d/value", current.pin_number );

		FILE* dir_stream;
		if( (dir_stream = fopen( dirPath, FILE_WRITE )) == NULL)
		{
			return DIR_OPEN_ERROR;
		}



		if( fwrite( current.direction, 1, strlen( current.direction ), dir_stream ) < 0 )
		{
			fclose( dir_stream );
			return DIR_WRITE_ERROR;
		}

		if( fclose( dir_stream ) != EOF )
		{
			return DIR_CLOSE_ERROR;
		}

		FILE* rw_stream;

		if( strcmp( OUT_STR, current.direction ) == 0 )
		{
			// read
		}
		if( strcmp( IN_STR, current.direction ) == 0 )
		{
			if( (rw_stream = fopen( rwPath, FILE_WRITE )) == NULL )
			{
				return WR_OPEN_ERROR;
			}

			char val_str [2];
			snprintf( val_str, 2, "%d", current.value );
			if( fwrite( val_str, 1, 2, rw_stream ) < 0 )
			{
				return WR_WRIE_ERROR;
			}
			if( fclose( rw_stream ) != EOF )
			{
				return WR_CLOSE_ERROR;
			}
}
