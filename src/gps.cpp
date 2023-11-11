#include "gps.h"
#include "lcd.h"


#define GPSSerial Serial2
#define RX2 2
#define TX2 3

Adafruit_GPS GPS(&GPSSerial);

long t0 = millis();
long t1 = millis();

void initializeGps(void) {
  GPSSerial.begin(9600, SERIAL_8N1, RX2, TX2);
  GPSSerial.println("$PMTK251,38400*27");  
  delay(100);
  GPSSerial.end();
  delay(100);
  GPSSerial.begin(38400, SERIAL_8N1, RX2, TX2);
  GPSSerial.println("$PMTK251,38400*27");  
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPSSerial.println(PMTK_SET_NMEA_UPDATE_10HZ);  
  delay(100);
 
  GPSSerial.println(PMTK_Q_RELEASE);
}


int loops = 0;
int nme = 0;

char buff5[30];

void handleGps(void) {

  char c = GPS.read();

  if(c == 0) {
    delay(1);
    return;
  }

  Serial.print(c);

  loops++;

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) 
      return;

    nme++;

    lcd1( gpsFix() );
    lcd2( gpsLat() );
    lcd3( gpsLon() );
    lcd4( gpsSpeed() );
    sprintf( buff5, "%06d %06d", loops, nme);
    lcd4( buff5 );
    updateLcd();
  }

 
  // t1 = millis();
  // Serial.printf( "%s | %s | %s | %08d | %08d | %08d \n", gpsLat(), gpsLon(), gpsSpeed(), loops, nme, t1 - t0 );
  // t0 = t1;
}

char buff1[100];

char* gpsFix(void) {
  // sprintf( buff1, "fix: %1d sat:%02d dop:%5.2f", (int)GPS.fix, GPS.satellites, GPS.PDOP );
  sprintf( buff1, "fix: %1d dop:%5.2f", (int)GPS.fix, GPS.PDOP );
  return buff1;
}

char buff2[100];
char noFix[20] = "pos: no fix";

char* gpsLat(void) {
  if (GPS.fix) {
    sprintf( buff2, "lat: %09.4f", GPS.latitude );
    return buff2;
  } else {
    return noFix;
  }
}


char buff3[100];
char* gpsLon(void) {
  if (GPS.fix) {
    sprintf( buff3, "lat: %09.4f", GPS.longitude );
    return buff3;
  } else {
    return noFix;
  }
}


char buff4[100];
char* gpsSpeed(void) {
  sprintf( buff4, "vel: %08.5f ", GPS.speed );
  return buff4;
}
