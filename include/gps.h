#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

void initializeGps(void);
void handleGps(void);

char* gpsFix(void);
char* gpsLat(void);
char* gpsLon(void);
char* gpsSpeed(void);

