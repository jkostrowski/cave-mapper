#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <RTClib.h>

#include "trace.h"

void gpsInitialize(void);
void gpsRead(void);
void gpsParse(void);

void gpsOnNmea( void (*f)(void) );
void gpsOnFix( void (*f)(void) );
void gpsOnFixGood( void (*f)(void) );

int gpsLog(char*);

char* gpsQuality(void);

// char* gpsFix(void);
// char* gpsLat(void);
// char* gpsLon(void);
// char* gpsSpeed(void);

DateTime gpsNow(void);

