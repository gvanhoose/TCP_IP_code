#include <stdio.h>
#include <stdlib.h>
#include "file_IO.h"

#define LIGHT_CONTROL_FILEPATH	"~/Desktop/On_Startup/light_control.txt"
#define READ_ONLY		"r"
#define WRITE_ONLY		"w"

#define LIGHT_OFF		0x0
#define LIGHT_ON		0x1
#define MASK_FIRST_BIT		-2

int getLightStatus()
{
	FILE* light_FD;

	if( (light_FD = fopen( LIGHT_CONTROL_FILEPATH, READ_ONLY )) == NULL )
	{
		return LIGHT_OFF;
	}

	char status_buffer [2];

	if( fread( status_buffer, 1, 2, light_FD ) != 2 )
	{
		fclose( light_FD );
		return LIGHT_OFF;
	}

	fclose( light_FD );

	if( *status_buffer != '0' && *status_buffer != '1' )
	{
		return LIGHT_OFF;
	}

	


/**
 * reads from the light control file (which should be wiped on startup)
 * if the file indicates the light is on, (will contain '1' char)
 *	turn light off
 *	rewrite light control file with '0' char
 * if the file indicates the light is off, (will contain '0' char)
 *	turn light on
 *	rewrite light control file with '1' char
 */
int main()
{
	

	exit( SUCCESS );
}
