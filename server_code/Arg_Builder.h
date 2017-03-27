#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * get_nargs(...) takes in a single char* and returns 1 + the number of occurrences of the ' ' char before the null-terminator or newline character.
 * The idea being that a ' ' char deliniates a new argument for a particular process call.
 */
int get_nargs( const char* );

/**
 * The purpose of build_arg(...) is to convert a series of arguments passed via TCP IPv4 connection, in char* format, to an array of char* which is parsed out according to ' ' characters.
 */
char** build_arg( const char* );
