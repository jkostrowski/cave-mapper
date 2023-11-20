#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <RTClib.h>

void gpsInitialize(void);
void gpsLoop(void);

char* getGps(void);

char* gpsFix(void);
char* gpsLat(void);
char* gpsLon(void);
char* gpsSpeed(void);

char* gpsQuality(void);

void gpsOnNmea( void (*f)(void) );
void gpsOnFix( void (*f)(void) );
void gpsOnFixGood( void (*f)(void) );

DateTime gpsNow(void);

