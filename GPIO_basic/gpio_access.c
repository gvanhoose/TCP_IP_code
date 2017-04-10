#include "gpio_access.h"

int GPIO_init( const int n_requests, struct Pin_Request* requests )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];

		int init_fd, pinStringLength;

		char pinNumberString [ PIN_STRING_MAX_LENGTH ];

		pinStringLength = snprintf( pinNumberString, PIN_STRING_MAX_LENGTH, "%d", current.pin_number);

		if( (init_fd = open( GPIO_INIT_PATH, O_WRONLY )) < 0 )
		{
//			log_error( GPIO_INIT_ERR_MSG );
			return GPIO_INIT_OPEN_ERROR;
		}

		if( write( init_fd, pinNumberString, pinStringLength ) < 0 )
		{
//			log_error( "GPIO init write error" );
			close( init_fd );
			return GPIO_INIT_WRITE_ERROR;
		}

		if( close( init_fd ) != 0 )
		{
//			log_error( "GPIO init close error" );
			return GPIO_INIT_CLOSE_ERROR;
		}
	}

	return SUCCESS:
}

int GPIO_release( int n_requests, struct Pin_Request* requests )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];

		int release_fd, pinStrLength;

		char pinString [ MAX_PIN_STRING_LENGTH ];

		if( (release_fd = open( GPIO_RELEASE_PATH, O_WRONLY )) < 0 )
		{
//			log_error( "GPIO release open error" );
			return GPIO_RELEASE_OPEN_ERROR;
		}

		pinStrLength = snprintf( pinString, MAX_PIN_STRING_LENGTH, "%d", current.pin_number );

		if( write( release_fd, pinString, pinStrLength ) < 0 )
		{
//			log_error( "GPIO release write error" );
			close( release_fd );
			return GPIO_RELEASE_WRITE_ERROR;
		}

		if( close( release_fd ) != 0 )
		{
//			log_error( "GPIO release close error" );
			return GPIO_RELEASE_CLOSE_ERROR;
		}
	}

	return SUCCESS;
}

int configurePinDirections( const int n_requests, struct Pin_Request* requests )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];
// check to ensure current is not null!
		char buffer [ STD_BUFFER_LENGTH ];

		int direction_fd, bytes_written;

		if( (direction_fd = open( DIRECTION_PATH_FOR_PIN( current.pin_number ), O_WRONLY )) < 0 );
		{
//			log_error( "Unable to access file descriptor for direction path" );
println( "configurePinDirection fail on open" );
			return( DIRECTION_OPEN_ERROR );	// \_ pick one
//			continue;			// /	or allow caller to choose... maybe...
		}

		if( write( direction_fd, current.direction, strlen(current.direction) ) < 0 )
		{
//			log_error( "Unable to set direction" );
println( "configurePinDirection fail on write" );
			close( direction_fd );
			return( DIRECTION_WRITE_ERROR );
//			continue;
		}

		if( close( direction_fd ) != 0 )
		{
//			log_error( "Unable to close pin directioin location" );
			return( DIRECTION_CLOSE_ERROR );
//			continue;
		}
	}

	return 0;
}

int executePinRequests( const int n_requests, struct Pin_Request* requests )
{
	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];

		char file_buffer [ PIN_IO_BUFFER_LENGTH ];
		memset( file_buffer, '\0', PIN_IO_BUFFER_LENGTH );

		int
			write_fd,
			read_eor_write = (strcmp( IN, current.direction))? O_WRONLY : O_RDONLY
			;
		if( (write_fd = open( READ_WRITE_PATH(current.pin_number), read_eor_write )) < 0 )
		{
//			log_error( "failed to open pin filepath for read or write request" );
			return( PIN_RW_OPEN_ERROR );
//			continue;	// testing only!
		}

		if( read_eor_write == O_WRONLY )
		{
			file_buffer[0] = current.value | 0x30;

			if( write( write_fd, file_buffer, 1) < 0 )
			{
				close( write_fd );
//				log_error( "failed to write a value to a pin" );
				return ( PIN_WRITE_ERROR );
//				continue;	// testing only!
			}
		}
		else if( read_eor_write == O_RDONLY )
		{
			if( read( write_fd, file_buffer, 1 ) < 0 )
			{
				close( write_fd );
//				log_error( "failed to read a value from a pin" );
				return( PIN_READ_ERROR );
//				continue;
			}
			current.value = atoi( file_buffer );
		}
		else
		{

/**
 * WARNING!!!
 * if the program is as well-tested and planned as I hope, this scope should never execute.
 *	if this scope does execute, then there is a problem in parsing args during this program's initial call!
 */
			close( write_fd );
//			log_error( "unkown request -- not read nor write.  Check to make sure improper arguments are identified" );
			return( UNKNOWN_PIN_REQUEST );
		}

		if( close( write_fd ) != 0 )
		{
//			log_error( "failed to close pin filepath after read or write" );
			return ( PIN_CLOSE_ERROR );
//			continue;
		}

	}

	return SUCCESS;
}

//int writePin( const int pinNumber, const int value )
//{
//printf( "Writing %d to pin %d", value, pinNumber );
//println("");
//
//	return -1;
//}

/**
 * args for this program shall proceed as follows:
 *	argv[i]		description			example
 *
 *	argv[0]		name of executable		"gpio_acces"
 *	argv[1]		number of pins being accessed	"4"
 *	argv[2]		pin#, direction, value		"1,OUT=HIGH"
 *	argv[3]		pin#, direction, value		"2,IN="
 *	argv[4]		pin#, direction, value		"3,OUT=LOW"
 * (has to be readable in the logs)
 */
int main( int argc, char* argv [] )
{
	if( strcmp( "./test", argv[ 0 ] ) == 0 )
	{
		println("");
		println( "NEW TEST" );
		int t;
		for( t = 0; t < argc; t++ )
		{
			printf( "%s ", argv[t] );	// when combined with the stdout redirect, this prints to file!
		}
		println("");
		printf("Result:\t");
	}
/*
 * commented out for testing only!
 */
//printf( "nargs:  %d", argc );
	if( argc < 3 )		// there must be at least 3 incoming args!
	{
//		log_error("Incorrect number of args for gpio_access");	// set up error logging for this
println( "argc < 3" );
		exit(PROGRAM_ERROR);
	}

	int pin_count;
	if( (pin_count = atoi( argv[ PIN_COUNT_INDEX ] )) == 0 )	// if pinCount is zero, the following loop will not run and no pins will be updated!
	{
//		log_error("No pin count provided.");
println( "invalid pin count -- non-numeric chars" );
		exit(PROGRAM_ERROR);
	}
// at this point, pin count is determined to be nonzero
	if( pin_count < 0 || pin_count > MAX_PIN_COUNT )		// checks for too high of a pin count (in reality, pin_count should be unsigned, but checking for negative values works just the same since valid values of pin_count are extremely low)
	{
//		log_error("Pin count is too high.");
println( "invalid pin count -- too high" );
		exit(PROGRAM_ERROR);
	}
printf( "pin count is %d", pin_count );
println("");

	struct Pin_Request pinRequests [ pin_count ];

// at this point, pin count is determined to be an allowable number
// now to service the pin requests.
	int i;
	for( i = 0; i < pin_count; i++ )
	{
		if( strcmp( "./test", argv[0] ) == 0 )
		{
			printf( "Pin Request %d:\t", i );
		}

		char* current = argv[ i + PIN_COUNT_INDEX + 1 ];
		char* rawPinNumber;
		char* rawDirection;
		char* rawWriteValue;

//printf( "Parsing over %s", current );
//println( "" );	// have to workaround not being able to use escape characters

		int pinNumber, toWrite;

		// get the pin number from current, if it isn't between current[0] and the ',', NULL is returned
		if( (rawPinNumber = strtok( current, PIN_NUMBER_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin number or incorrectly formatted." );
println( "missing pin # or bad format" );
			exit( PROGRAM_ERROR );
		}

		// get the direction of the pin from current; if it isn't between ',' and '=' NULL is returned
		if( (rawDirection = strtok( NULL, PIN_DIRECTION_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin direction or incorrectly formatted." );
println( "missing direction or misspelled or bad format" );
			exit( PROGRAM_ERROR );
		}

		// get the write value of the pin from current; it may not be there so testing needs to be a little more complex
		rawWriteValue = strtok( NULL, PIN_TO_WRITE_DEMARKER );

		if(
			( strcmp( OUT, rawDirection ) == 0 && rawWriteValue == NULL )	// rawWriteValue cannot be NULL if rawDirection contains OUT
			||
			( strcmp( IN,  rawDirection ) == 0 && rawWriteValue != NULL )	// rawWriteValue mus be NULL if rawDirection is IN
		)	// rawDirection may contain erroneous information, so these calls to strcmp are not reliable to that end just yet.
		{
//			log_error( "Write value specified with request to Read, or no Write value specified with request to Write" );
println( "HIGH or LOW specified on IN, or no value specified on OUT" );
			exit( PROGRAM_ERROR );
		}

//		if( *rawPinNumber == '0' &&  strlen( rawPinNumber ) == 1 )	// important to differentiate between a request for pin 0 and a error return from "int atoi(char*)"
//		{
//			pinNumber = 0;
//		}
//		else
//		{
			if( (pinNumber = atoi( rawPinNumber )) == 0 )
			{
//				log_error( "Invalid pin number." );	// because rawPinNumber was tested on whether it specified pin 0, the only way the conditional could be true is if atoi returned zero, meaning rawPinNumber contained a non-numeric char
println( "bad pin number" );
				exit( PROGRAM_ERROR );
			}
			if( pinNumber < MIN_PIN_NUMBER || pinNumber > MAX_PIN_NUMBER )	// also test for specific pin numbers that are either too high or low
			{
//				log_error( "Invalid pin number. (must be a pin between 2-27)" );
println( "pin number too high or low" );
				exit( PROGRAM_ERROR );
			}
//		}
// at this point, pinNumber is guaranteed to identify a specific, valid pin on the RPi

		pinRequests[ i ].pin_number = pinNumber;

// verify rawDirection contains either "in" or "out"

		if( strcmp(IN, rawDirection) == 0 )
		{
// confirmed rawDirection == "in"
			if( rawWriteValue != NULL )
			{
//				log_error( "pin read request specifies write value; unknown if intent was to read or write pin" );
println( "Request for pin IN cannot specify a write value" );
				exit( PROGRAM_ERROR );
			}
			pinRequests[ i ].direction = rawDirection;
			pinRequests[ i ].value = UNREAD;
println( "pin in request has been accepted" );
			continue;
		}
		else if( strcmp(OUT, rawDirection) == 0)
		{
// confirmed rawDirection == "out"
			if( rawWriteValue == NULL )
			{
//				log_error( "pin write request does not specify write value; unknown if intent was to read or write pin" );
println( "Request for pin OUT must specify a write value" );
				exit( PROGRAM_ERROR );
			}
			int isNotLow;
			if(
				(isNotLow = strcmp( "LOW", rawWriteValue ) != 0)
				&&
				strcmp( "HIGH",  rawWriteValue ) != 0
			  )
			{
//				log_error( "write value not correctly specified" );
println( "Write value for pin OUT not correctly specified" );
				exit( PROGRAM_ERROR );
			}
			pinRequests[ i ].direction = rawDirection;
			pinRequests[ i ].value = isNotLow? HIGH : LOW;	// cannot guarantee the value of isNotLow will be only 1 or 0
//			strcpy( &(pinRequests[i].direction), rawDirection );
println( "pin out request has been accepted" );
			continue;
		}
		else
		{
// confirmed rawDirection is invalid
//			log_error( "Invalid pin direction" );
println( "direction does not accurately specify \"in\" or \"out\"" );
			exit( PROGRAM_ERROR );
		}

	}

	if( GPIO_init( pin_count, pinRequests ) != 0 )
	{
//		log_error( "GPIO was not successfully initialized for these requests" );
		exit( GPIO_INIT_FAIL );
	}
	if( configurePinDirections( pin_count, pinRequests ) != 0 )
	{
//		log_error( "GPIO directions were not successfully set" );
		exit( GPIO_DIR_CONFIG_FAIL );
	}
	if( executePinRequests( pin_count, pinRequests ) != 0 )
	{
/		log_error( "GPIO could not execute these pin requests" );
		exit( GPIO_EXEC_PIN_FAIL );
	}
	if( GPIO_release( pin_count, pinRequests ) != 0 )
	{
//		log_error( "GPIO could not release pins after executing pin requests" );
		exit( GPIO_RELEASE_FAIL );
	}

	return SUCCESS;
}
