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

#define MIN_ARGS	3

#define PIN_COUNT_INDEX	1
#define MAX_PIN_COUNT	26
#define MIN_PIN_NUMBER	2
#define MAX_PIN_NUMBER	27


#define PIN_NUMBER_DEMARKER	":"
#define PIN_DIRECTION_DEMARKER	"="
#define PIN_TO_WRITE_DEMARKER	"\0"

#define SUCCESS			0
#define ERROR			-1
#define PIN_COUNT_ERROR		-2

struct Pin_Request
{
	char* direction;
	int pin_number, value;
};

int executeRequests( struct Pin_Request[], int );

int buildPinRequests( int, char*[], struct Pin_Request[], int );

#endif	// _GPIO_H_
