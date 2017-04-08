#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define IN		0
#define OUT		1
#define LOW		0
#define HIGH		1

#define PIN_COUNT_INDEX	1 // which index within argv contains the number of pins affected by this request
#define MAX_PIN_COUNT	29	// 40 pins, 9 are constant voltage (2 each 3.3VDC and 5VDC, 5 0VDC/Ground), 2 are reserved)
#define MAX_PIN_NUMBER	26

#define PROGRAM_ERROR	-1
#define LBAD_PIN_ARG  -2

#define GPIO_INIT_PATH		"/sys/class/gpio/export"
#define GPIO_INIT_ERR_MSG	"Failed to open sys/class/gpio/export for writing\n"

#define GPIO_RELEASE_PATH	"/sys/class/gpio/unexport"
#define GPIO_RELEASE_ERR_MSG	"Failed to open sys/class/gpio/unexport for writing\n"

#define DIRECTION_PATH_FOR_PIN(x)	"/sys/class/gpio/gpio%d/direction\n",x
#define ERR_MSG_DIRECTION_PATH(x)	"Failed to open path for pin %d\n",x
#define ERR_MSG_SET_DIRECTION(x)	"Failed to set direction for pin %d\n",x

#define READ_WRITE_PATH(x)	"/sys/class/gpio/gpio%d/value\n",x
#define READ_OPEN_EROR(x)	"On read:  Failed to open path for pin %d\n",x
#define READ_ERROR(x)		"On read:  Failed to read value for pin %d\n",x
#define WRITE_OPEN_ERROR(x)	"On write:  Failed to open path for pin %d\n",x
#define WRITE_ERROR(x)		"On write:  Failed to write to pin %d\n",x

int extractPinNumber( char*, int);
