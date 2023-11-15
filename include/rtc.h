#include <Arduino.h>
#include <RTClib.h>

void rtcInitialize(void);
char* rtcLog(void);
char* rtcTime(void);

void rtcSet(DateTime);
DateTime rtcTimestamp(void);
