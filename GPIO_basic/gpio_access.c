#include "gpio_access.h"

int writePin( const int pinNumber, const int value )
{
	

	return 0;
}

/**
 * args for this program shall proceed as follows:
 *	argv[i]		description			example
 *
 *	argv[0]		name of executable		"gpio_acces"
 *	argv[1]		number of pins being accessed	"4"
 *	argv[2]		pin#, direction, value		"1,OUT;HIGH"
 *	argv[3]		pin#, direction, value		"2,IN;"
 *	argv[4]		pin#, direction, value		"3,OUT;LOW"
 * (has to be readable in the logs)
 */
int main( int argc, char* argv [] )
{

/*
 * commented out for testing only!
 */
	if( argc < 3 )		// there must be at least 3 incoming args!
	{
//		log_error("Incorrect number of args for gpio_access");	// set up error logging for this
		exit(PROGRAM_ERROR);
	}

	int pin_count;
	if( (pin_count = atoi( argv[ PIN_COUNT_INDEX ] )) == 0 )	// if pinCount is zero, the following loop will not run and no pins will be updated!
	{
//		log_error("No pin count provided.");
		exit(PROGRAM_ERROR);
	}
// at this point, pin count is determined to be nonzero
	if( pin_count < 0 || pin_count > MAX_PIN_COUNT )		// checks for too high of a pin count (in reality, pin_count should be unsigned, but checking for negative values works just the same since valid values of pin_count are extremely low)
	{
//		log_error("Pin count is too high.");
		exit(PROGRAM_ERROR);
	}
// at this point, pin count is determined to be an allowable number
// now to service the pin requests.
	int i;
	for( i = PIN_COUNT_INDEX + 1; i < pin_count; i++ )
	{
		char* current = argv[ i ];
		char* rawPinNumber;
		char* rawDirection;
		char* rawWriteValue;

		int pinNumber, toWrite;

		// get the pin number from current, if it isn't between current[0] and the ',', NULL is returned
		if( (rawPinNumber = strtok( current, PIN_NUMBER_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin number or incorrectly formatted." );
			exit( PROGRAM_ERROR );
		}

		// get the direction of the pin from current; if it isn't between ',' and ';' NULL is returned
		if( (rawDirection = strtok( NULL, PIN_DIRECTION_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin number or incorrectly formatted." );
			exit( PROGRAM_ERROR );
		}

		// get the write value of the pin from current; it may not be there so testing needs to be a little more complex
		rawWriteValue = strtok( NULL, PIN_TO_WRITE_DEMARKER );

		if(
			( strcmp( RAW_STR_WRITE, rawDirection ) == 0 && rawWriteValue == NULL )	// rawWriteValue cannot be NULL if rawDirection contains OUT
			||
			( strcmp( RAW_STR_READ,  rawDirection ) == 0 && rawWriteValue != NULL )	// rawWriteValue mus be NULL if rawDirection is IN
		)	// rawDirection may contain erroneous information, so these calls to strcmp are not reliable to that end just yet.
		{
//			log_error( "Write value specified with request to Read, or no Write value specified with request to Write" );
			exit( PROGRAM_ERROR );
		}

		if( *rawPinNumber == '0' &&  strlen( rawPinNumber ) == 1 )	// important to differentiate between a request for pin 0 and a error return from "int atoi(char*)"
		{
			pinNumber = 0;
		}
		else
		{
			if( (pinNumber = atoi( rawPinNumber )) == 0 )
			{
//				log_error( "Invalid pin number." );		// because rawPinNumber was tested on whether it specified pin 0, the only way the conditional could be true is if atoi returned zero, meaning rawPinNumber contained a non-numeric char
				exit( PROGRAM_ERROR );
			}
			if( pinNumber < 0 || pinNumber > MAX_PIN_NUMBER )	// also test for specific pin numbers that are either too high or low
			{
//				log_error( "Invalid pin number. (must be a pin between 0-29)" );
				exit( PROGRAM_ERROR );
			}
		}
// at this point, pinNumber is guaranteed to identify a specific, valid pin on the RPi

// now to check rawDirection and rawWriteValue

		int dirComparedToWrite;

		if(
			(dirComparedToWrite = strcmp( RAW_STR_WRITE, rawDirection ) != 0)
			&&
			strcmp( RAW_STR_READ, rawDirection ) != 0
		  )	// ensures rawDirection contains either "READ" or "WRITE" and nothing else!
		{
//			log_error( "Arg does not specify read or write properly." );
			exit( PROGRAM_ERROR );
		}

		if( dirComparedToWrite )		// almost-guaranteed to be a read if conditional is true
		{
			if( rawWriteValue != NULL )	// if 
			{
//				log_error( "Write value not left blank.  Unknown if intent was to read from the pin or write to that pin." );
				exit( PROGRAM_ERROR );
			}
			// TODO:  code to read from pins goes here.
			// NB:  The current scope of the project does not require any reading from pins.
			continue;
		}
// at this point, almost-guaranteed to have a write

		if( rawWriteValue == NULL )
		{
//			log_error( "Write value is not specified.  Unknown if intent was to write a value to a pin or to read from that pin" );
			exit( PROGRAM_ERROR );
		}

		toWrite = ( strcmp( RAW_STR_HIGH, rawToWrite ) == 0 )? HIGH : ( strcmp( RAW_STR_LOW, rawToWrite ) == 0 )? LOW : -1;

		if( toWrite < 0 )
		{
//			log_error( "Invalid write value." );
			exit( PROGRAM_ERROR );
		}

		writePin( pinNumber, toWrite );
	}

	return 0;
}
