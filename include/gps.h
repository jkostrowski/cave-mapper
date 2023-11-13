#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

void gpsInitialize(void);
bool gpsSinglePass(void);

char* getGps(void);

char* gpsFix(void);
char* gpsLat(void);
char* gpsLon(void);
char* gpsSpeed(void);

