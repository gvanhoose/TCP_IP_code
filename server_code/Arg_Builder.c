#include "Arg_Builder.h"

void dbug()
{
	printf( "Reaching%c", NEW_LINE );
}

int get_nargs( const char* rawCmd )
{
	int nargs = 1, i = 0;

	char current;

	while( (current = rawCmd[i++]) != NULL_TERM && current != NEW_LINE )
	{
		if ( current == ' ' )
		{
			nargs++;
		}
	}

	return nargs;
}

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







void build_args( char* argsOutput[], char* blankString, char* rawCmd, int nargs )
//char** build_args( char* argsOutput[], const char* rawCmd, int nargs )
{

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
}


/**
 * For testing only!
 */
int main()
{
	char* rawArg = "Hello World! How are you?\0";

	int nargs = get_nargs( rawArg );

	char* args [nargs];

	build_args( args, rawArg, nargs );

	print_args( nargs, args );
//	printf( "%d%c", get_nargs( "Hello World! How are you?\n" ), NEW_LINE );

	return 0;
}
