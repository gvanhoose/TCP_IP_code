#include "gpio_access.h"

/**
 * args for this program shall proceed as follows:
 *	argv[i]		description			example
 *	argv[0]		name of executable		"gpio_acces"
 *	argv[1]		number of pins being accessed	"4"
 *	argv[2]		pin#, direction, value		"1,OUT,HIGH"
 *	argv[3]		pin#, direction, value		"2,IN,,"
 *	argv[4]		pin#, direction, value		"3,OUT,LOW"
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
	if( pin_count < 0 OR pin_count > MAX_PIN_COUNT )
	{
//		

//	printf( ERR_MSG_DIRECTION_PATH(7) );

//	int pin = 8;
//	printf( ERR_MSG_DIRECTION_PATH(pin) );

//	char msg [] = ERR_MSG_DIRECTION_PATH(pin);
//	printf( msg );

//	char msg [] = ERR_MSG_SET_DIRECTION(3);
//	printf( msg );

	return 0;
}
