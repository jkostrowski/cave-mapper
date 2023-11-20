#include "gps.h"


#define GPSSerial Serial2
#define RX2 2
#define TX2 3

Adafruit_GPS GPS(&GPSSerial);

unsigned long t0 = millis();
unsigned long t1 = millis();

bool onFix1Sent = false;
bool onFix2Sent = false;

int32_t homeLo = 0;  // Longitude_fixed of the location when fix was reported
int32_t homeLa = 0;  // Lattitude_fixed of the location when fix was reported

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
  GPSSerial.end();
  delay(100);
  GPSSerial.begin(38400, SERIAL_8N1, RX2, TX2);
  GPSSerial.println("$PMTK251,38400*27");  
  // GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGAGSA);
  GPSSerial.println(PMTK_SET_NMEA_UPDATE_10HZ);  
  delay(100);
 
  GPSSerial.println(PMTK_Q_RELEASE);
}

//===================================================================

bool gpsSinglePass(void) {  // returns true when it is safe to write to Serial.

  if (!GPS.available())
    return false;

  char c = GPS.read();
  Serial.print(c);

  bool fullMessage = GPS.newNMEAreceived() && GPS.parse(GPS.lastNMEA()); 

  if (!onFix1Sent && fullMessage && GPS.fix ) {
    onFix1();
    onFix1Sent = true;
  }

  if (!onFix2Sent && fullMessage && GPS.satellites >= 8) {
    onFix2();
    onFix2Sent = true;
    homeLo = GPS.longitude_fixed;
    homeLa = GPS.latitude_fixed;
  }

  return fullMessage;
}

//===================================================================

void gpsLoop(void) {
  unsigned long x0 = millis();
  for (int i=0; i<500; i++) {
      if (gpsSinglePass()) {
        onNmea();
      }
    }
  unsigned long x1 = millis();

  // Serial.print( "loop: = " );
  // Serial.println( x1-x0 );
}

//===================================================================

char buff0[140];   
char* getGps(void) {
  sprintf( buff0, "F,%1d,LA,%10d,LO,%10d,HD,%6.2f,VD,%6.2f,PD,%6.2f,V,%6.2f,S,%2d,DA,% 10d,DO,% 10d,Q,%1d"
  , (int) GPS.fix
  , GPS.latitude_fixed 
  , GPS.longitude_fixed
  , GPS.HDOP
  , GPS.VDOP
  , GPS.PDOP
  , GPS.speed 
  , GPS.satellites
  , onFix2Sent ? GPS.latitude_fixed - homeLa : 0
  , onFix2Sent ? GPS.longitude_fixed - homeLo : 0
  , GPS.fixquality
  );
  return buff0;
}

char noFix[] = "no fix";
char buff1[100];
char* gpsFix(void) {
  sprintf( buff1, "F:%1d S:%2d", (int)GPS.fix, GPS.satellites );
  return buff1;
}

char buff2[100];
char* gpsLat(void) {
  if (GPS.fix) {
    sprintf( buff2, "LA:%09.4f", GPS.latitude );
    return buff2;
  } else {
    return noFix;
  }
}

char buff3[100];
char* gpsLon(void) {
  if (GPS.fix) {
    sprintf( buff3, "LO: %09.4f", GPS.longitude );
    return buff3;
  } else {
    return noFix;
  }
}


char buff4[30];
char* gpsSpeed(void) {
  sprintf( buff4, "VE: %05.2f", GPS.speed );
  return buff4;
}


char buff5[30];
char* gpsQuality(void) {
  sprintf( buff5, "H:%5.2f V:%5.2f", GPS.HDOP, GPS.VDOP );
  return buff5;
}


DateTime gpsNow(void) {
  return DateTime( GPS.year, GPS.month, GPS.day, GPS.hour, GPS.minute, GPS.seconds ) - TimeSpan(6 * 3600);
}