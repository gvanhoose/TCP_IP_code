#include "gpio_access.h"

void dbug()
{
	printf( "Reaching\n" );
}

/**
 * takes an string arg in
 *	number , string ; format
 * and returns the int value of number
 * returns LBAD_PIN_ARG on error
 *	if index of ',' is greater than 3, is bad pin arg
 *	if index of ',' is 0, is bad pin arg
 */
int extractPinNumber( char* raw, int rawLength )
{
	char substring [2];	// guaranteed to be some number between 0-40, hence only two digits are required to represent it.
	char* end_substr = substring;

	char current;

	if( *raw < 0x30 || *raw > 0x39 )	// checks for non-numerical characters at the start of the arg
	{
//dbug();
//		log_error( "Invalid gpio pin request" );
		return LBAD_PIN_ARG;
	}
//dbug();
	while( (current = *(raw++)) != ',' )
	{
		if( current < 0x30 || current > 0x39 || (end_substr - substring >= 2) )	// checks for non-numerical characters and pin numbers that are longer than they should be
		{			
//dbug();
//			log_error("Bad gpio pin request");
			return LBAD_PIN_ARG;
		}
		*(end_substr++) = current;
	}
//dbug();
	return atoi( substring );
}

char* extractDirection( char* raw, int raw_length )
{
	char* output = raw;

	while( *(raw++) != ',' );

	if( (raw - output) < 2

/**
 * args for this program shall proceed as follows:
 *	argv[i]		description			example
 *
 *	argv[0]		name of executable		"gpio_acces"
 *	argv[1]		number of pins being accessed	"4"
 *	argv[2]		pin#, direction, value		"1,OUT;HIGH"
 *	argv[3]		pin#, direction, value		"2,IN;"
 *	argv[4]		pin#, direction, value		"3,OUT;LOW"
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

		int cur_length;

		int pinNumber = extractPinNumber( current , (cur_length = strlen( current )) );

		if( pinNumber < 0 )
		{
//			log_error("bad pin number, cannot proceed with GPIO request" );
			exit(PROGRAM_ERROR);
		}

//		current += ( pinNumber >= 10 )? 2:1;

		char* direction = extractDirection( current, cur_length );

		// TODO:  set up a function to extract the pin direction and another to extract the value to be thus written, if applicable
	}
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
