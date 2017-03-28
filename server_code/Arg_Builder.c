#include "Arg_Builder.h"

void dbug()
{
	printf( "Reaching\n" );
}

/**
 * Returns the number of valid arguments given in a command line.
 *	treats consecutive spaces as a single space
 *	returns 0 if given an empty string or a string consisting of only spaces.
 */
int get_nargs( char* rawArg, int argLength )
{
	int nargs = 0;
	
	char* end = rawArg + argLength;
	
	int spaceEncountered = TRUE;
	
	while( rawArg < end )
	{
		nargs += ( spaceEncountered & *rawArg != ' ' );
		spaceEncountered = *(rawArg++) == ' ';
	}

	return nargs;
}

void build_args( char* target[], char* rawArg, int argLength, int nargs )
{
	if( nargs == 0 )
	{
		return;
	}
	
	char* end = rawArg-- + argLength;
	
	char toAdd, previous = ' ';
	
	char* current_string = *target;
	
	while( ++rawArg < end )
	{
		toAdd = ( *rawArg == ' ' )? '\0' : *rawArg;
		
		if( previous == '\0' && toAdd == '\0' )
		{
			previous = toAdd;
			continue;
		}
		
		previous = toAdd;
		
		*(current_string++) = toAdd;
		
		if( toAdd == '\0' )
		{
			target += ( toAdd == '\0' );
			
			current_string = *target;
		}
	}

}


/**
 * For testing only!
 */
int main()
{
	char* tester = "Hello World!  HEY YOU GUYSSSS! ! !";
//	char* tester = "        ";
//	char* tester = "";


	int tLen = strlen( tester ), nargs = get_nargs( tester, tLen );

	printf( "Test:\t\"%s\"\nnargs:\t%d\n", tester, nargs );

	char* args [ nargs ];

	int i;
	for( i = 0; i < nargs; i++ )
	{
		args[i] = malloc( tLen + 1 );
		memset( args[i], '\0', tLen + 1 );
	}

	build_args( args, tester, tLen, nargs );

	printf( "Reading args:\n" );

	for( i = 0; i < nargs; i++ )
	{
		printf( "\targs[%d] =\t\"%s\"\n", i, args[i] );
	}
	
	return 0;
}
