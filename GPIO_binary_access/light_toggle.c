#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIGHT_PIN_VAL_FILEPATH		"/sys/class/gpio/gpio4/value"
#define READ_ONLY			"r"
#define WRITE_ONLY			"w"
//#define LIGHT_ON_PATH			"/home/pi/On_Startup/lightOn"
//#define LIGHT_OFF_PATH			"/home/pi/On_Startup/lightOff"

int main()
{
	FILE* pinFD;

	if( (pinFD = fopen( LIGHT_PIN_VAL_FILEPATH, READ_ONLY )) == NULL )
	{
		// error handling here
		exit( -1 );
	}

	char pinVal [2];

	if( fread( pinVal, 1, 2, pinFD ) < 0 )
	{
		// more error handling here
		fclose( pinFD );
		exit( -1 );
	}

	fclose( pinFD );

	pinVal[1] = '\n';

	if( (pinFD = fopen( LIGHT_PIN_VAL_FILEPATH, WRITE_ONLY )) == NULL )
	{
		// error handling here
		exit( -1 );
	}

	if( pinVal[0] == '1' )
	{
		pinVal[0] = '0';
	}
	else if( pinVal[0] == '0' )
	{
		pinVal[0] = '1';
	}

	if( fwrite( pinVal, 1, 2, pinFD ) < 0 )
	{
		// error handling here
		fclose( pinFD );
		exit( -1 );
	}

	fclose( pinFD );

	return 0;
}
