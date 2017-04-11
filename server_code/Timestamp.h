#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TIMESTAMP_ERROR -1
#define BASE_YEAR       1900
#define MONTH_OFFSET    1

char* getTimestamp();

#endif  // _TIMESTAMP_H_
