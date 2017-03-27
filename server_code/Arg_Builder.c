#include "Arg_Builder.h"

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

	return 0;
}

int main()
{
	printf( "%d", get_nargs( "Hello World!" ) );

	return 0;
}
