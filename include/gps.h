#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <RTClib.h>

void gpsInitialize(void);
void gpsLoop(void);

void gpsOnNmea( void (*f)(void) );
void gpsOnFix( void (*f)(void) );
void gpsOnFixGood( void (*f)(void) );

char* getGps(void);
char* gpsQuality(void);

// char* gpsFix(void);
// char* gpsLat(void);
// char* gpsLon(void);
// char* gpsSpeed(void);

DateTime gpsNow(void);

