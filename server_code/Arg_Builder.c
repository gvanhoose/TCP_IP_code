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
	
//	int nargs = 1, i = 0;
//
//	char current;
//
//	while( (current = rawCmd[i++]) != NULL_TERM && current != NEW_LINE )
//	{
//		if ( current == ' ' )
//		{
//			nargs++;
//		}
//	}

	return nargs;
}

/*
 * obsolete functions follow
 *
void print_args(int nargs, char* args [] )
{
	printf( "nargs:  %d%c", nargs, NEW_LINE );

	int i;

	for( i = 0; i < nargs; i++ )
	{
		printf( "%s%c", args[ i ], NEW_LINE );
	}
}

void init_args( struct args* s_args, char* rawArg )
{

	s_args->raw_arg = rawArg;

	s_args->nargs = get_nargs( rawArg );

	char* arg_vec [ s_args->nargs ];

	s_args

}
 *
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
/*
 * obsolete function definition follows
 *
//	char* argsOutput [ nargs ];

	int rawArgLength = strlen( rawCmd );

//	char copyCmd [ rawArgLength + 1 ];	// need one extra space for an initial delimiter!

//	copyCmd[0] = NULL_TERM;

	strcpy( copyCmd + 1, rawCmd );

	int i, cmdLength = strlen( rawCmd ), currentArg = 0;

	argsOutput[ currentArg++ ] = rawCmd;

	for( i = 0; i < cmdLength; i++ )
	{

//		printf( "%s%c", argsOutput[ currentArg - 1 ], NEW_LINE );

		if( rawCmd[ i ] == ' ' )
		{
//dbug();
//			rawCmd[ i ] = NULL_TERM;
//dbug();
			argsOutput[ currentArg ] = rawCmd + i + 1;
//dbug();
		}

//		if( rawCmd[i] == ' ' )
//		{
//			rawCmd[i] = NULL_TERM;
//		}
//		if( rawCmd[i] == NULL_TERM )
//		{
//			argsOutput[ currentArg++ ] = rawCmd + i + 1;
//		}
	}

//	print_args( nargs, argsOutput );

//	return argsOutput;
 *
 */
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
/*
 *
	char* rawArg = "Hello World! How are you?\0";

	int nargs = get_nargs( rawArg );

	char* args [nargs];

	build_args( args, rawArg, nargs );

	print_args( nargs, args );
//	printf( "%d%c", get_nargs( "Hello World! How are you?\n" ), NEW_LINE );
 *
 */
	return 0;
}
