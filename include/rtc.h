#include <Arduino.h>
#include <RTClib.h>

void rtcInitialize(void);
int rtcLog(char*);
char* rtcTime(void);

void rtcSet(DateTime);
DateTime rtcTimestamp(void);
