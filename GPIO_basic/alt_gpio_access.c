#include "gpio_access.h"

/**
 * args for this program shall proceed as follows:
 *	argv[i]		description			example
 *
 *	argv[0]		name of executable		"gpio_acces"
 *	argv[1]		number of pins being accessed	"4"
 *	argv[2]		pin#, direction, value		"1,OUT,HIGH"
 *	argv[3]		pin#, direction, value		"2,IN,"
 *	argv[4]		pin#, direction, value		"3,OUT,LOW"
 */
int main( int argc, char* argv [] )
{

/*
 * commented out for testing only!
 */
	if( argc < 3 )
	{
//		log_error("Incorrect number of args for gpio_access");	// set up error logging for this
		exit(PROGRAM_ERROR);
	}

	int pin_count;
	if( (pin_count = atoi( argv[ PIN_COUNT_INDEX ] )) == 0 )
	{
//		log_error("No pin count provided.");
		exit(PROGRAM_ERROR);
	}
// at this point, pin count is determined to be nonzero
	if( pin_count < 0 || pin_count > MAX_PIN_COUNT )
	{
//		log_error("Pin count is too high.");
		exit(PROGRAM_ERROR);
	}
// at this point, pin count is determined to be an allowable number

	int i;
	for( i = PIN_COUNT_INDEX + 1; i < pin_count; i++ )
	{
		char* current = argv[ i ];
		char* rawPinNumber;
		char* rawDirection;
		char* rawWriteValue;

		int pinNumber, direction, toWrite;

		// check pin number inthe arg
		if( (rawPinNumber = strtok( current, PIN_NUMBER_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin number or incorrectly formatted." );
			exit( PROGRAM_ERROR );
		}

		if( (rawDirection = strtok( NULL, PIN_DIRECTION_DEMARKER )) == NULL )
		{
//			log_error( "Invalid argument, missing pin number or incorrectly formatted." );
			exit( PROGRAM_ERROR );
		}
		rawWriteValue = strtok( NULL, PIN_TO_WRITE_DEMARKER );	// this can be NULL if rawDirection is "IN", requires special testing!

		if(
			( strcmp( rawDirection, "OUT" ) == 0 && rawWirteValue == NULL )
			||
			( strcmp( rawDirection, "IN" ) == 0 && rawWriteValue != NULL )
		)
		{
//			log_error( "Inbound requests must not specify a write value; similarly, outbound requests must specify a write value" );
			exit( -1 );
		}

		if( *rawPinNumber == '0' &&  strlen( rawPinNumber ) == 1 )	// important to differentiate between a request for pin 0 and a error return from "int atoi(char*)"
		{
			pinNumber = 0;
		}
		else
		{
			if( (pinNumber = atoi( rawPinNumber )) == 0 )
			{
//				log_error( "Invalid pin number." );
				exit( PROGRAM_ERROR );
			}
			if( pinNumber < 0 || pinNumber > MAX_PIN_COUNT )	// change MAX_PIN_COUNT to MAX_PIN_NUMBER, also change the macro to 26, not 29
			{
//				log_error( "Invalid pin number. (must be a pin between 0-29)" );
				exit( PROGRAM_ERROR );
			}
		}

// directional configuration of GPIO shoudl go here

// pin number is guaranteed to be valid from this poin onward
		switch( rawToWrite )
		{
			case WRITE_HIGH:
			{
				if( strcmp( rawDirection, DIRECTION_OUT ) )
				{
//					log_error( "GPIO input cannot specify a write HIGH value; unknown if intent was to write active high to pin %d or to read said pin.", pinNumber );
					exit( PROGRAM_ERROR );
				}
				// TODO:  write the "void writeHigh( int pinNumber )" function. (same for WRITE_LOW case
//				writeHigh( pinNumber );
				break;
			}
			case WRITE_LOW:
			{
				if( strcmp( rawDirection, DIRECTION_OUT ) )
				{
//					log_error( "GPIO input cannot specify a write LOW value.  Unknown if intent was to write active low to pin %d, or to read from said pin.", pinNumber );
					exit( PROGRAM_ERROR );
				}
//				writeLow( pinNumber );
				break;
			}
			default:
			{
				if( strcmp( rawDirection, NULL ) )
				{
					break;
				}
//				log_error( "Invalid pin request.  Unable to process 
			}
		}

	}
/*
 *
 */

/*
 * testing for "int extractDirection( char*, int );"
 */
	char tester [] = "21,IN;";		// correct input
//	char tester [] = "21,endTheFed;";
//	char tester [] = "IN,21;HIGH";
//	char tester [] = ";";
//	char tester [] = ",";
//	char tester [] = "";
//	char tester [] = ",;";
//	char tester [] = ";,";
//	char tester [] = "21,;HIGH";
//	char tester [] = "21:,IN";
//	char tester [] = ",IN;";
//	char tester [] = "15,OUT;HIGH";		// correct input
//	char tester [] = "15;OUT,HIGH";
//	char tester [] = "15,artificialInflationIsTheftAlso;HIGH";
//	char tester [] = "OUT,15;HIGH";
//	char tester [] = "OUT;15,HIGH";
//	char tester [] = "HIGH,15;OUT";
//	char tester [] = "HIGH;15,OUT";
//	char tester [] = "HIGH,OUT;15";
//	char tester [] = "HIGH;OUT,15";
//	char tester [] = "15;OUT;HIGH";
//	char tester [] = "15,OUT,HIGH";
//	char tester [] = "15,out;HIGH";
/*
 *
 */

/*
 * testing for "int extractPinNumber( char*, int );"
 *	all tests passed!
 *
//	char tester [] = "3953,Hello;world";
//	char tester [] = "320,Hello;world";	// did not work; update:  fixed this problem!
//	char tester [] = "1;Hello,world";
//	char tester [] = ";";
//	char tester [] = ",";
//	char tester [] = "Hello,;world";
//	char tester [] = "a,Hello;world";
	char tester [] = "12,Hello;world";	// correct input case
//	char tester [] = "";
//	char tester [] = "1Hello;world";
//	char tester [] = "1 ,Hello;world";
//	char tester [] = ",Hello;world";

	int n, tLen = strlen( tester );

	n = extractPinNumber( tester, tLen );

	printf( "%d\n", (n + 1) );
 *
 *
 */



/*
 * testing to make sure the macros work
 *	they do
 *
//	printf( ERR_MSG_DIRECTION_PATH(7) );

//	int pin = 8;
//	printf( ERR_MSG_DIRECTION_PATH(pin) );

//	char msg [] = ERR_MSG_DIRECTION_PATH(pin);
//	printf( msg );

//	char msg [] = ERR_MSG_SET_DIRECTION(3);
//	printf( msg );
 *
 */
	return 0;
}
