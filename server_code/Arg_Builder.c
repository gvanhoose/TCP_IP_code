#include "Arg_Builder.h"

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

/**
 * Parses a single string into char*[] format
 * This allows compatibility between TCP data transmissions and the argument for calls to execv()
 */
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
		toAdd = ( *rawArg == ' ' )? NULL_TERM : *rawArg;

		if( previous == NULL_TERM && toAdd == NULL_TERM )
		{
			previous = toAdd;
			continue;
		}

		previous = toAdd;

		*(current_string++) = toAdd;

		if( toAdd == NULL_TERM )
		{
			target += ( toAdd == NULL_TERM );

			current_string = *target;
		}
	}

}
