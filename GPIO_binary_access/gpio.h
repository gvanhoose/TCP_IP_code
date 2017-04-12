#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN		"in"
#define OUT		"out"

#define UNREAD		-1
#define ACTIVE_LOW	0
#define ACTIVE_HIGH	1

#define LOW_STR		"LOW"
#define HIGH_STR	"HIGH"

#define FILE_WRITE	"w"
#define FILE_READ	"r"

#define MIN_ARGS	1	// seems kinda pedantic, but this was at one point defined as 0x3

#define PIN_COUNT_INDEX	1
#define MAX_PIN_COUNT	26
#define MIN_PIN_NUMBER	2
#define MAX_PIN_NUMBER	27


#define PIN_NUMBER_DEMARKER	":"
#define PIN_DIRECTION_DEMARKER	"="
#define PIN_TO_WRITE_DEMARKER	"\0"

#define TRUE	1
#define FALSE	0

#define SUCCESS				0
#define ERROR				-1
#define PIN_REQUEST_ERROR(x)		((( -1 ) << 8) | x )	// x is the pin number, is returned in the least-significant byte of the overall error code, which is still technically negative#define IS_NON_REQUEST_ERROR(z)	((z & 0xff < 0xff )	// 
#define UNMASK_BAD_REQUEST_INDEX(y)	( y & 0x0ff )	// strips away the superfluous 2nd through Most-Significant bytes, leaving only the pin number that contains bad information
#define IS_NON_REQUEST_ERROR(z)		((z & 0xff < 0xff )	// if it is a non-request-related error, then the entire least-significant byte will be 0xff -- there aren't enough pins to even get close to this value!
#define PIN_COUNT_ERROR			-2

struct Pin_Request
{
	char* direction;
	int pin_number, value;
};

int executeRequests( struct Pin_Request[], int );

int buildPinRequests( int, char*[], struct Pin_Request[], int );

#endif	// _GPIO_H_
