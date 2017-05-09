#ifndef _COMBINE_ARGS_H_
#define _COMBINE_ARGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combine_args.c"

#define SPACE_STR	" "
#define NULL_STR	"\0"
#define NULL_TERM	'\0'

#define NO_ARGS		-1

int getTotalArgLength( int n_args, char* args [] );
void buildSingleArg( char* target, int tarLen, int nargs, char* args [] );

#endif
