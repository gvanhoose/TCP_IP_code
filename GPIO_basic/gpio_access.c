#include "gpio_access.h"

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


	while( (current = *(raw++)) != ',' )
	{
		if( current < 0x30 || current > 0x39 || (end_substr - substring > 2) )	// checks for non-numerical characters and pin numbers that are longer than they should be
		{
//			log_error("Bad gpio pin request");
			return LBAD_PIN_ARG;
		}
		*(end_substr++) = current;
	}

	return atoi( substring );
}
/**
 * args for this program shall proceed as follows:
 *	argv[i]		description			example
 *	argv[0]		name of executable		"gpio_acces"
 *	argv[1]		number of pins being accessed	"4"
 *	argv[2]		pin#, direction, value		"1,OUT;HIGH"
 *	argv[3]		pin#, direction, value		"2,IN;"
 *	argv[4]		pin#, direction, value		"3,OUT;LOW"
 */
int main( int argc, char* argv [] )
{
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


	char tester [] = "1,Hello;world";

	int n, tLen = strlen( tester );

	n = extractPinNumber( tester, tLen );

	printf( "%d\n", n );

//	int i;
//	for( i = 0; i < pin_count; i++ )
//	{
//		char* current = argv[ i + PIN_COUNT_INDEX + 1 ];
//
//		if(






//	printf( ERR_MSG_DIRECTION_PATH(7) );

//	int pin = 8;
//	printf( ERR_MSG_DIRECTION_PATH(pin) );

//	char msg [] = ERR_MSG_DIRECTION_PATH(pin);
//	printf( msg );

//	char msg [] = ERR_MSG_SET_DIRECTION(3);
//	printf( msg );

	return 0;
}
