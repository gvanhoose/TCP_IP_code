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

#define MINIMUM_REQUIRED_ARGS	3	// this is the minimum count of argument tokens that MUST be passed to the gpio_access program

#define FILE_WRITE	"w"
#define FILE_READ	"r"

#define PIN_STRING_MAX_LENGTH	3	// 2 for pin number, 1 for null term
#define PIN_VALUE_STR_LENGTH	1	// only need 1

#define GPIO_PATH_BUFFER_LENGTH	40

//#define MAX_EXPORT_ATTEMPTS	5

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

//#define PIN_IO_BUFFER_LENGTH	2

#define GPIO_RELEASE_PATH	"/sys/class/gpio/unexport"
//#define GPIO_RELEASE_ERR_MSG	"Failed to open sys/class/gpio/unexport for writing\n"

//#define DIRECTION_PATH_FOR_PIN(x)	"/sys/class/gpio/gpio%d/direction\n",(int)x
//#define ERR_MSG_DIRECTION_PATH(x)	"Failed to open path for pin %d\n",x
//#define ERR_MSG_SET_DIRECTION(x)	"Failed to set direction for pin %d\n",x

//#define READ_WRITE_PATH(x)	"/sys/class/gpio/gpio%d/value\n",(int)x
//#define READ_OPEN_ERROR(x)	"On read:  Failed to open path for pin %d\n",x
//#define READ_ERROR(x)		"On read:  Failed to read value for pin %d\n",x
//#define WRITE_OPEN_ERROR(x)	"On write:  Failed to open path for pin %d\n",x
//#define WRITE_ERROR(x)		"On write:  Failed to write to pin %d\n",x

struct Pin_Request
{
	int pin_number, value, readWrite_fd;
	char* direction;
};

int GPIO_init( const int, struct Pin_Request* );		// may need to be moved to another program

int configurePinDirections( const int, struct Pin_Request* );

/**
 * PARAMS:
 *	const int n_requests		The number of pins being requested for GPIO access
 *	struct Pin_Request* requests	An array of type struct Pin_Request*, which contains all information and space required for reading or writing
 * RETURNS:
 *	local int i		the total number of requests that were successfully serviced (shold be equal to n_requests arg)
 *	PIN_RW_OPEN_ERROR	if /sys/class/gpio/gpio#/value cannot be found
 *	PIN_WRITE_ERROR		if the current request's value is not successfully written to the pin
 *	PIN_READ_ERROR		if the specified pin's value cannot be successfully read
 *	UNKNOWN_PIN_REQUEST	if the current request is unspecified vis-a-vis read or write
 *	PIN_CLOSE_ERROR		if /sys/class/gpio/gpio#/value cannot be closed properly
 * NOTES:
 *	It is somewhat expected that the most-common erroroneous return will be PIN_RW_OPEN_ERROR
 *		This happens if the pin number has not be exported.
 *		This is remedied by exporting the pin from either the command line, or attempting to direct the program to update the /sys/class/gpio/export file directly
 *	This function can take in a dynamic number of read and write requests
 *		All pins specified in these requests must be exported in /sys/class/gpio/export BEFORE this function is called
 * ON SUCCESS:
 *	In the OS:
 *		/sys/class/gpio/gpio# (where # is the pin number) will carry a /direction and a /value, which can be read from or written to
 *		All file descriptors are disposed within the scope of this function
 *		if a read is performed, the specific pin request in requests will have its value variable updated from UNREAD (-1) to either HIGH (1) or LOW (0)
 *			it is the duty of the caller to provide a means for this function to relay the pin values back
 *			suggest having a file for this program to dump input values once they are read
 *	On the hardware:
 *		if a write is performed, the pin specified by the individual request will carry the new output voltage
 *		if a read is performed, the behavior of the pin's voltage may not be predictable, but typical behavior with simple GPIO dictates that its voltage should remain unaffected by any read
 * ON FAILURE:
 *	In the OS:
 *		Presently, the entire function will be exited.
 *		A solution which attempts to rectify certain errors is in development, which will preserve the program's flow as best as possible without it quitting in the middle of the requests list
 *	On the hardware:
 *		Any GPIO requests which are serviced BEFORE an error is encountered will remain unchanged; similarly, re-running this function with the entire list of pin requests will not affect the output, since they are identical requests
 *		Any GPIO requests which come AFTER an error is encountered remain unserviced, unless a means of correcting errors where possible is implemented
 */
int executePinRequests( const int, struct Pin_Request* );
int GPIO_release( const int, struct Pin_Request* );		// may need to be moved to another program

/**
 * the following may be de-commented for debugging purposes!
 *

// this can be used to help measure printed string lengths
//	it prints a '|' char on every tenth character space
//	up to the length specified by maxLength
void printHashMarks(int maxLength)
{
	int i;
	for( i = 0; i < maxLength; i++ )
	{
		if( (i+1) % 10 != 1 )
		{
			printf( " " );
			continue;
		}
		printf( "|" );
	}
	printf( "\n" );
}

// want to print a simple message?
// does manually typing in the new line char '\n' take up too much time?
// or maybe your keyboard doesn't allow you to easily type the new line char?
// well have we got the function for you!
// This function takes in a string, prints it, AND takes care of printing the '\n' char so you don't have to!
void println( const char* toPrint)
{
	printf( "%s\n", toPrint );
};

// this prints a simple message:  "Reaching" and starts a new line
// this is particularly useful if you want to track program flow during debugging
void dbug()
{
	printf( "Reaching\n" );
}
 *
 *
 */

#endif	// _GPIO_ACCESS_H_
