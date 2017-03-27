#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Common_Macros.h"

struct args
{
	char* raw_arg;
	char* argv [];
	int nargs;
};

int get_nargs( const char* rawCmd );
void init_args( struct args*, const char* );
void build_args( char*[], char* rawCmd, int );
