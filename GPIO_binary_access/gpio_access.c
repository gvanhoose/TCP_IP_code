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
