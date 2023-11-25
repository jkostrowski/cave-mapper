#include "gps.h"


#define GPSSerial Serial2
#define RX2 2
#define TX2 3

Adafruit_GPS GPS(&GPSSerial);

unsigned long t0 = millis();
unsigned long t1 = millis();

bool onFix1Sent = false;
bool onFix2Sent = false;

int32_t homeLo = 0;   // Longitude_fixed of the location when fix was reported
int32_t homeLa = 0;   // Lattitude_fixed of the location when fix was reported

// ==================================================================

void nop(void) {}

void (*onFix1)(void) = &nop;
void (*onFix2)(void) = &nop;
void (*onNmea)(void) = &nop;

// ==================================================================

void gpsOnNmea( void (*f)(void) ) {
  onNmea = f;
}

void gpsOnFix( void (*f)(void) ) {
  onFix1 = f;
}

void gpsOnFixGood( void (*f)(void) ) {
  onFix2 = f;
}

// ==================================================================

void gpsInitialize(void) {
  GPSSerial.begin(9600, SERIAL_8N1, RX2, TX2);
  GPSSerial.println("$PMTK251,38400*27");  
  delay(100);
  GPSSerial.updateBaudRate(38400);
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGAGSA);
  GPSSerial.println(PMTK_SET_NMEA_UPDATE_10HZ);  
  delay(100);
}

//===================================================================

void gpsRead(void) {
  // TRACE("gpsRead");

  if (!GPS.available())
    return;

  char c = GPS.read();
  Serial.print(c);
}

void gpsParse(void) {
  // TRACE("gpsParse");
  bool fullMessage = GPS.newNMEAreceived() && GPS.parse(GPS.lastNMEA()); 
  Serial.printf( "\n>> new nmea: %d, parsing |%s\n", (int) fullMessage, GPS.lastNMEA() );

  if (!onFix1Sent && fullMessage && GPS.fix ) {
    onFix1();
    onFix1Sent = true;
  }

  if (!onFix2Sent && fullMessage && GPS.satellites >= 8) {
    homeLo = GPS.longitude_fixed;
    homeLa = GPS.latitude_fixed;
    onFix2();
    onFix2Sent = true;
  }

  if (fullMessage) 
    onNmea();
}

//===================================================================

int gpsLog(char* log) {
  TRACE("gpsLog");

  return sprintf( log, "F,%1d,LA,% 12d,LO,% 12d,HD,%6.2f,VD,%6.2f,PD,%6.2f,V,%6.2f,S,%2d,Q,%1d,DA,% 12d,DO,% 12d"
  , (int) GPS.fix
  , GPS.latitude_fixed 
  , GPS.longitude_fixed
  , GPS.HDOP
  , GPS.VDOP
  , GPS.PDOP
  , GPS.speed 
  , GPS.satellites
  , GPS.fixquality
  , onFix2Sent ? GPS.latitude_fixed  - homeLa : 0
  , onFix2Sent ? GPS.latitude_fixed  - homeLo : 0
  );

}

char buff5[30];
char* gpsQuality(void) {
  TRACE("gpsQuality");
  sprintf( buff5, "H:%6.2f V:%6.2f", GPS.HDOP, GPS.VDOP );
  return buff5;
}

DateTime gpsNow(void) {
  return DateTime( GPS.year, GPS.month, GPS.day, GPS.hour, GPS.minute, GPS.seconds ) - TimeSpan(6 * 3600);
}