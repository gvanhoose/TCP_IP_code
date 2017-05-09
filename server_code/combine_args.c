#include "combine_args.c"

int getTotalArgLength( int n_args, char* args [] )
{
	if( n_args <= 1 )
	{
		return NO_ARGS;	// if no args are passed, the concatenated string is null
	}

	int i, argLen = 0;
	for( i = 1; i < n_args; i++ )
	{
		argLen += strlen( args[ i ] );
	}

	return n_args + argLen - 2;	// n strings in args (accounting for spaces), each of argLen length
}

void buildSingleArg( char* target, int tarLen, int nargs, char* args [] )
{
	memset( target, NULL_CHAR, tarLen );

	int i;
	for( i = 1; i < nargs; i++ )
	{
		strcat( target, args[ i ] );
		strcat( target, SPACE_STR );
	}

	target[ tarLen ] = NULL_TERM;

}
