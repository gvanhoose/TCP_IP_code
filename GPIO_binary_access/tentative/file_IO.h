#include <stdio.h>
#include <stdlib.h>
//#include "Server_Log.h"

#define LIGHT_FILEPATH "light_status.txt"
#define READ_ONLY "r"
#define WRITE_ONLY "w"

int getLightStatus()
{
	FILE* light_FD;

	if( (light_FD = fopen( LIGHT_FILEPATH, READ_ONLY )) == NULL )
	{
		logError( "could not open light file on getLightStatus" );
		return LLIGHT_FILE_IO_ERROR;
	}

	char status [2];
	status[0] = '\0';	// \_ probably quicker than memset...
	status[1] = '\0';	// /

	fread( status, 1, 2, light_FD );

	return (int) ((*status)& 0x0F);

}

int updateLightStatus( int on_off )
{
	FILE* light_FD;

	if( (light_FD = openFile( LIGHT_FILEPATH, WRITE_ONLY )) == NULL )
	{
		logError( "could not open light file on updateLightStatus" );
		return LLIGHT_FILE_IO_ERROR;
	}

	if( on_off & -2 != 0 )			// a check to ensure on_off is ONLY 0x1 or 0x0
	{
		logError( "invalid input on updateLightStatus" );
		return LBAD_INPUT;
	}

	char status [2];
	status[0] = (char) (on_off | 0x30);	// \_ probably quicker than snprintf...
	status[1] = '\0';			// /



	fwrite( status, 1, 2, light_FD );

	return SUCCESS;

}

