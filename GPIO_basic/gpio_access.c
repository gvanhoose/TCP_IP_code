#include "gpio_access.h"

int GPIO_init( const int n_requests, struct Pin_Request* requests )
{
	printf( "\nReaching GPIO_init\n" );

	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];

		// open the file descriptor for the export
		FILE* init_fd = fopen( GPIO_INIT_PATH, FILE_WRITE );
		if( init_fd == NULL )
		{
//			log_error( GPIO_INIT_ERR_MSG );
			printf( "\topen error\n" );
			return GPIO_INIT_OPEN_ERROR;
		}

		char pinNumberString [ PIN_STRING_MAX_LENGTH ];
//		memset( pinNumberString, '\0', PIN_STRING_MAX_LENGTH );

		int pinStringLength = snprintf( pinNumberString, PIN_STRING_MAX_LENGTH, "%d", current.pin_number);

//		int errRetVal;
		// none of these work!  UGH!
		if( fprintf( init_fd, "%d", current.pin_number ) < 0)
//		if( fwrite( pinNumberString, 1, pinStringLength, init_fd ) != pinStringLength )
//		if( (errRetVal = fwrite( pinNumberString, 1, pinStringLength, init_fd )) != pinStringLength )
		{
//			log_error( "GPIO init write error" );
			printf( "\twrite error\n" );
			fclose( init_fd );
			return GPIO_INIT_WRITE_ERROR;
		}

		int clRetVal;
		if( (clRetVal = fclose( init_fd )) != 0 && clRetVal != EOF )	// need to know if EOF is necessarily a bad thing...
		{
//			log_error( "GPIO init close error" );
//			printf( "\tclose returned: %d\n\tis EOF?\t%s\n", clRetVal, (clRetVal == EOF)? "yes" : "no" );
			printf( "\tclose error\n" );
			return GPIO_INIT_CLOSE_ERROR;
		}
	}

	printf( "\tSuccessful initialization!\n" );
	return SUCCESS;
}

int GPIO_release( int n_requests, struct Pin_Request* requests )
{
	printf( "\nReaching GPIO_release\n" );

	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];

		FILE* release_fd = fopen( GPIO_RELEASE_PATH, FILE_WRITE );
		int pinStrLength;

		char pinString [ PIN_STRING_MAX_LENGTH ];

		if( release_fd == NULL )
		{
//			log_error( "GPIO release open error" );
			return GPIO_RELEASE_OPEN_ERROR;
		}

		pinStrLength = snprintf( pinString, PIN_STRING_MAX_LENGTH, "%d", current.pin_number );

		if( fwrite( pinString, 1, pinStrLength, release_fd ) < 0 )
		{
//			log_error( "GPIO release write error" );
			fclose( release_fd );
			return GPIO_RELEASE_WRITE_ERROR;
		}

		int clRetVal;
		if( (clRetVal = fclose( release_fd )) != 0 && clRetVal != EOF )
		{
//			log_error( "GPIO release close error" );
			return GPIO_RELEASE_CLOSE_ERROR;
		}
	}

	printf( "\tSuccessful Release!\n" );
	return SUCCESS;
}

int configurePinDirections( const int n_requests, struct Pin_Request* requests )
{
	printf( "\nReaching configurePinDirections\n" );

	int i;
	for( i = 0; i < n_requests; i++ )
	{
		struct Pin_Request current = requests[i];
// check to ensure current is not null!
//		char buffer [ STD_BUFFER_LENGTH ];

//		FILE* direction_fd;

		char dirPath [ GPIO_PATH_BUFFER_LENGTH ];
		memset( dirPath, '\0', GPIO_PATH_BUFFER_LENGTH );

		int pathLength = snprintf( dirPath, GPIO_PATH_BUFFER_LENGTH, "/sys/class/gpio/gpio%d/direction", current.pin_number );

//		printOnTens();
//		printf( "%s\n", dirPath );
//		printf( "pathLength = %d\n", pathLength);

//		FILE* direction_fd = fopen( "/sys/class/gpio/gpio4/direction", FILE_WRITE );
		FILE* direction_fd = fopen( dirPath, FILE_WRITE );

//		int exportAttempts = MAX_EXPORT_ATTEMPTS;

//		while( direction_fd == NULL && exportAttempts-- > 0)
		if( direction_fd == NULL )
		{
//			log_error( "Unable to access file descriptor for direction path" );
//			printf( "\tdirection_fd == 0x%x\n", direction_fd );
//			printf( "\tis direction_fd null? %s\n", (direction_fd == NULL)? "yes":"no" );
			printf( "\tfail on open\n" );
			return DIRECTION_OPEN_ERROR;

/*
 * this probably didn't work because this process has a different file handle from bash
 *	therefore, using echo to output to a file would have totally different behavior
 *
			printf( "\tPin %d has not been exported.  Exporting now\n\tAttempt:  %d of %d\n", current.pin_number, exportAttempts, MAX_EXPORT_ATTEMPTS );
			char exUnexPath [ GPIO_PATH_BUFFER_LENGTH ];

			// first use exUnexPath to unexport the pin
			memset( exUnexPath, '\0', GPIO_PATH_BUFFER_LENGTH );
			snprintf( exUnexPath, GPIO_PATH_BUFFER_LENGTH, "echo %d > /sys/class/gpio/unexport", current.pin_number );

			printf( "about to execute \"%s\"\n", exUnexPath );
			system( exUnexPath );

			// then use exUnexPath to export the pin
			memset( exUnexPath, '\0', GPIO_PATH_BUFFER_LENGTH );
			snprintf( exUnexPath, GPIO_PATH_BUFFER_LENGTH, "echo %d > /sys/class/gpio/export", current.pin_number );

			printf( "about to execute \"%s\"\n", exUnexPath );
			system( exUnexPath );
 *
 */

		}

/*
		if( exportAttempts <= 0 )
		{
//			log_error( "could not open direction path for pin after several attempts" );
			return DIRECTION_OPEN_ERROR;
		}
*/
		if( fwrite( current.direction, 1, strlen(current.direction) + 1, direction_fd ) < 0 )
		{
//			log_error( "Unable to set direction" );
			printf( "\tfail on write\n" );
			fclose( direction_fd );
			return( DIRECTION_WRITE_ERROR );
//			continue;
		}

		int clRetVal;
		if( (clRetVal = fclose( direction_fd )) != 0 && clRetVal != EOF )
		{
//			log_error( "Unable to close pin directioin location" );
			printf( "\tfail on close\n" );
			return( DIRECTION_CLOSE_ERROR );
//			continue;
		}
	}

	printf( "\tSuccessful direction setting!\n" );

	return 0;
}

int executePinRequests( const int n_requests, struct Pin_Request* requests )
{
	printf( "\nReaching executePinRequests\n" );

	int i;
	for( i = 0; i < n_requests; i++ )
	{

		struct Pin_Request current = requests[i];

//		memset( file_buffer, '\0', PIN_VALUE_STR_LENGTH );

		char* read_eor_write = (strcmp( IN, current.direction))? FILE_WRITE : FILE_READ;

		char rwPath [ GPIO_PATH_BUFFER_LENGTH ];
//		memset( rwPath, '\0', 35 );

		int rwStrLength = snprintf( rwPath, GPIO_PATH_BUFFER_LENGTH, "/sys/class/gpio/gpio%d/value",current.pin_number );

		FILE* write_fd = fopen( rwPath, read_eor_write );

		if( write_fd == NULL )
		{
//			log_error( "failed to open pin filepath for read or write request" );
			printf( "\tfail on open\n" );
			return( PIN_RW_OPEN_ERROR );
		}

//		printf( "\tread or write?  %s\n", read_eor_write );

		char value_buffer [ 2 ];

		if( strcmp( read_eor_write, FILE_WRITE ) == 0 )
		{
			int pinStrLength = snprintf( value_buffer, 2, "%s", current.value? "1":"0" );

//			printf( "\thigh or low?  %s\n", value_buffer );
//			printf( "\tcurrent.value = %d\n", current.value );

			if( fwrite( value_buffer, 1, pinStrLength, write_fd ) < 0 )
			{
				fclose( write_fd );
//				log_error( "failed to write a value to a pin" );
				printf( "\tfail on write\n" );
				return ( PIN_WRITE_ERROR );
			}
		}
		else if( strcmp( read_eor_write, FILE_READ ) == 0 )
		{

			if( fread( value_buffer, 1, PIN_VALUE_STR_LENGTH, write_fd ) < 0 )
			{
				fclose( write_fd );
//				log_error( "failed to read a value from a pin" );
				printf( "\tfail on read\n" );
				return( PIN_READ_ERROR );
//				continue;
			}
			current.value = atoi( value_buffer );
		}
		else
		{

/**
 * WARNING!!!
 * if the program is as well-tested and planned as I hope, this scope should never execute.
 *	if this scope does execute, then there is a problem in parsing args during this program's initial call!
 */
			fclose( write_fd );
//			log_error( "unkown request -- not read nor write.  Check to make sure improper arguments are identified" );
			printf( "\tfail on unknown request\n" );
			return( UNKNOWN_PIN_REQUEST );
		}

		int clRetVal;
		if( (clRetVal = fclose( write_fd )) != 0 && clRetVal != EOF )
		{
//			log_error( "failed to close pin filepath after read or write" );
			printf( "\tfail on close\n" );
			return ( PIN_CLOSE_ERROR );
//			continue;
		}

	}

	printf( "\tSuccessful pin execution!\n" );

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
//	if( system( NULL ) 

	if( strcmp( "./test", argv[ 0 ] ) == 0 )
	{
		printf( "============================\nNEW TEST\n" );
		int t;
		for( t = 0; t < argc; t++ )
		{
			printf( "%s ", argv[t] );	// when combined with the stdout redirect, this prints to file!
		}
		printf("\nResult:\t");
	}

	if( argc < MINIMUM_REQUIRED_ARGS )		// there must be at least 3 incoming args!
	{
//		log_error("Incorrect number of args for gpio_access");	// set up error logging for this
		printf( "argc < 3\n" );
		exit(PROGRAM_ERROR);
	}

	int pin_count;
	if( (pin_count = atoi( argv[ PIN_COUNT_INDEX ] )) == 0 )	// if pinCount is zero, the following loop will not run and no pins will be updated!
	{
//		log_error("No pin count provided.");
		printf( "invalid pin count -- non-numeric chars\n" );
		exit(PROGRAM_ERROR);
	}
// at this point, pin count is determined to be nonzero
	if( pin_count < 0 || pin_count > MAX_PIN_COUNT )		// checks for too high of a pin count (in reality, pin_count should be unsigned, but checking for negative values works just the same since valid values of pin_count are extremely low)
	{
//		log_error("Pin count is too high.");
		printf( "invalid pin count -- too high\n" );
		exit(PROGRAM_ERROR);
	}
//	printf( "pin count is %d\n", pin_count );

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
			printf( "missing pin # or bad format\n" );
			exit( PROGRAM_ERROR );
		}

		// get the direction of the pin from current; if it isn't between ',' and '=' NULL is returned
		if( (rawDirection = strtok( NULL, PIN_DIRECTION_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin direction or incorrectly formatted." );
			printf( "missing direction or misspelled or bad format\n" );
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
			printf( "HIGH or LOW specified on IN, or no value specified on OUT\n" );
			exit( PROGRAM_ERROR );
		}

		if( (pinNumber = atoi( rawPinNumber )) == 0 )
		{
//			log_error( "Invalid pin number." );	// because rawPinNumber was tested on whether it specified pin 0, the only way the conditional could be true is if atoi returned zero, meaning rawPinNumber contained a non-numeric char
			printf( "bad pin number\n" );
			exit( PROGRAM_ERROR );
		}
		if( pinNumber < MIN_PIN_NUMBER || pinNumber > MAX_PIN_NUMBER )	// also test for specific pin numbers that are either too high or low
		{
//			log_error( "Invalid pin number. (must be a pin between 2-27)" );
			printf( "pin number too high or low\n" );
			exit( PROGRAM_ERROR );
		}
// at this point, pinNumber is guaranteed to identify a specific, valid pin on the RPi

		pinRequests[ i ].pin_number = pinNumber;

// verify rawDirection contains either "in" or "out"

		if( strcmp(IN, rawDirection) == 0 )
		{
// confirmed rawDirection == "in"
			if( rawWriteValue != NULL )
			{
//				log_error( "pin read request specifies write value; unknown if intent was to read or write pin" );
				printf( "Request for pin IN cannot specify a write value\n" );
				exit( PROGRAM_ERROR );
			}
			pinRequests[ i ].direction = rawDirection;
			pinRequests[ i ].value = UNREAD;
			printf( "pin in request has been accepted\n" );
/**
 * In the future, if input from GPIO is needed, this is where it will go
 */
			continue;
		}
		else if( strcmp(OUT, rawDirection) == 0)
		{
// confirmed rawDirection == "out"
			if( rawWriteValue == NULL )
			{
//				log_error( "pin write request does not specify write value; unknown if intent was to read or write pin" );
				printf( "Request for pin OUT must specify a write value\n" );
				exit( PROGRAM_ERROR );
			}
			int isNotLow;
			if(
				(isNotLow = strcmp( RAW_STR_LOW, rawWriteValue ) != 0)
				&&
				strcmp( RAW_STR_HIGH,  rawWriteValue ) != 0
			  )
			{
//				log_error( "write value not correctly specified" );
				printf( "Write value for pin OUT not correctly specified\n" );
				printf( "rawWriteValue == \"%s\"\n", rawWriteValue );
				exit( PROGRAM_ERROR );
			}
			pinRequests[ i ].direction = rawDirection;
			pinRequests[ i ].value = isNotLow? HIGH : LOW;	// cannot guarantee the value of isNotLow will be only 1 or 0
//			strcpy( &(pinRequests[i].direction), rawDirection );
			printf( "pin out request has been accepted\n" );
			continue;
		}
		else
		{
// confirmed rawDirection is invalid
//			log_error( "Invalid pin direction" );
			printf( "direction does not accurately specify \"in\" or \"out\"\n" );
			exit( PROGRAM_ERROR );
		}

	}

	printf( "About to start actual GPIO access\n" );

/*
 * The following conditional blocks of code are commented-out because the pins need to be initialized ("exported") elsewhere.
	if( GPIO_init( pin_count, pinRequests ) != 0 )
	{
//		log_error( "GPIO was not successfully initialized for these requests" );
		exit( GPIO_INIT_FAIL );
	}
 *
 */
	if( configurePinDirections( pin_count, pinRequests ) != 0 )
	{
		GPIO_release( pin_count, pinRequests );
//		log_error( "GPIO directions were not successfully set" );
		exit( GPIO_DIR_CONFIG_FAIL );
	}
	if( executePinRequests( pin_count, pinRequests ) != 0 )
	{
		GPIO_release( pin_count, pinRequests );
//		log_error( "GPIO could not execute these pin requests" );
		exit( GPIO_EXEC_PIN_FAIL );
	}
/*
 * calling this turns the specified pins off
	if( GPIO_release( pin_count, pinRequests ) != 0 )
	{
//		log_error( "GPIO could not release pins after executing pin requests" );
		exit( GPIO_RELEASE_FAIL );
	}
 *
 */
	printf( "GPIO access programming successfully completed\n" );

	return SUCCESS;
}
