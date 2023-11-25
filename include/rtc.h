#include <Arduino.h>
#include <RTClib.h>

#include "trace.h"

void rtcInitialize(void);
int rtcFullLog(char*);
char* rtcTimeLog(void);

void rtcSet(DateTime);
DateTime rtcTimestamp(void);
