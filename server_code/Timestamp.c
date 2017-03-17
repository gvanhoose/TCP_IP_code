#include "Timestamp.h"

/**
 * char* target is laid out as such:
 *  target[0] hour
 *  target[1] minute
 *  target[2] second
 *  target[3] month
 *  target[4] day
 *  target[5] year
 *
 */
int setTimeStamp( int* target )
{
  time_t rawTime;
  struct tm* timeInfo;
  
  time( &rawTime );
  
  timeInfo = localtime( &rawTime );
  
  target[0] = timeInfo->tm_hour;
  target[1] = timeInfo->tm_min;
  target[2] = timeInfo->tm_sec;
  target[3] = timeInfo->tm_mon + MONTH_OFFSET;
  target[4] = timeInfo->tm_mday;
  target[5] = timeInfo->tm_year + BASE_YEAR;
  
  return 0;
}
