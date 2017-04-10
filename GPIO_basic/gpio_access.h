#ifndef _GPIO_ACCESS_H_
#define _GPIO_ACCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define IN		"in"
#define OUT		"out"
#define LOW		0
#define HIGH		1
#define UNREAD		-1

#define PIN_STRING_MAX_LENGTH	3	// 2 for pin number, 1 for null term
#define PIN_VALUE_STR_LENGTH	1	// only need 1





#define RAW_STR_HIGH		"HIGH"
#define RAW_STR_LOW		"LOW"

#define PIN_NUMBER_DEMARKER	","
#define PIN_DIRECTION_DEMARKER	"="
#define PIN_TO_WRITE_DEMARKER	"\0"	// apparently strtok(...) will allow a null-term to be used

#define PIN_COUNT_INDEX	1	// which index within argv contains the number of pins affected by this request
#define MAX_PIN_COUNT	26	// 40 pins, 12 are constant voltage (2 each 3.3VDC and 5VDC, 8 0VDC/Ground), 2 are reserved for I2C ID EEPROM)
#define MIN_PIN_NUMBER	2	// there is no GPIO01
#define MAX_PIN_NUMBER	27	// GPIO pin numbers are continuous on [2,27]

#define SUCCESS			0	// how does one define success?
#define PROGRAM_ERROR		0	// de-comment this if testing with "make test_all"
#ifndef PROGRAM_ERROR
#define PROGRAM_ERROR		-1	// 
#endif	// PROGRAM_ERROR
#define LBAD_PIN_ARG		-2
#define GPIO_INIT_OPEN_ERROR	-3
#define GPIO_INIT_WRITE_ERROR	-4
#define GPIO_INIT_CLOSE_ERROR	-5
#define GPIO_RELEASE_OPEN_ERROR	-6
#define GPIO_RELEASE_WRITE_ERROR -7
#define GPIO_RELEASE_CLOSE_ERROR -8
#define DIRECTION_OPEN_ERROR	-9
#define DIRECTION_WRITE_ERROR	-10
#define DIRECTION_CLOSE_ERROR	-11
#define PIN_RW_OPEN_ERROR	-12
#define PIN_WRITE_ERROR		-13
#define PIN_READ_ERROR		-14
#define PIN_CLOSE_ERROR		-15
#define UNKNOWN_PIN_REQUEST	-16
#define GPIO_INIT_FAIL		-17
#define GPIO_DIR_CONFIG_FAIL	-18
#define GPIO_EXEC_PIN_FAIL	-19
#define GPIO_RELEASE_FAIL	-20

#define GPIO_INIT_PATH		"/sys/class/gpio/export"


#define PIN_IO_BUFFER_LENGTH	2

#define GPIO_RELEASE_PATH	"/sys/class/gpio/unexport"
//#define GPIO_RELEASE_ERR_MSG	"Failed to open sys/class/gpio/unexport for writing\n"

#define DIRECTION_PATH_FOR_PIN(x)	"/sys/class/gpio/gpio%d/direction\n",x
//#define ERR_MSG_DIRECTION_PATH(x)	"Failed to open path for pin %d\n",x
//#define ERR_MSG_SET_DIRECTION(x)	"Failed to set direction for pin %d\n",x

#define READ_WRITE_PATH(x)	"/sys/class/gpio/gpio%d/value\n",x
//#define READ_OPEN_ERROR(x)	"On read:  Failed to open path for pin %d\n",x
//#define READ_ERROR(x)		"On read:  Failed to read value for pin %d\n",x
//#define WRITE_OPEN_ERROR(x)	"On write:  Failed to open path for pin %d\n",x
//#define WRITE_ERROR(x)		"On write:  Failed to write to pin %d\n",x

struct Pin_Request
{
	int pin_number, value, readWrite_fd;
	char* direction;
};

int configurePinDirections( const int, struct Pin_Request* );
int executePinRequests( const int, struct Pin_Request* );

int writePin(const int, const int);

void println( const char* toPrint)
{
	printf( "%s\n", toPrint );
};

void dbug()
{
	printf( "Reaching\n" );
}


#endif	// _GPIO_ACCESS_H_
