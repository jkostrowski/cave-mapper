#include "gps.h"
#include "lcd.h"


#define GPSSerial Serial2
#define RX2 3
#define TX2 4

Adafruit_GPS GPS(&GPSSerial);

long t0 = millis();
long t1 = millis();

void initializeGps(void) {

  GPSSerial.begin(9600, SERIAL_8N1, RX2, TX2);

  //  GPSSerial.println("$PMTK251,57600*2C");  //set baud rate to 57600
  //  GPSSerial.println("$PMTK251,38400*27");  //set baud rate to 38400
  //  GPSSerial.println("$PMTK251,19200*22");  //set baud rate to 19200
  //  GPSSerial.println("$PMTK251,9600*17");   //set baud rate to 9600
 
  GPSSerial.println("$PMTK251,19200*22");  //set baud rate to 19200
  delay(100);
  GPSSerial.end();
  delay(100);

  GPSSerial.begin(19200, SERIAL_8N1, RX2, TX2);
  GPSSerial.println("$PMTK251,19200*22");  //set baud rate to 19200
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPSSerial.println(PMTK_SET_NMEA_UPDATE_10HZ);  // 10 Hz update rate
  
  delay(500);
  
  //  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  //  GPSSerial.println(PMTK_SET_NMEA_UPDATE_1HZ);
  //  GPS.sendCommand(PGCMD_ANTENNA);
  //  GPS.begin(57600);
 
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
 
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  
  // GPSSerial.println(PMTK_Q_RELEASE);
}

int loops = 0;
int nme = 0;

char buff5[30];

void handleGps(void) {

  char c = GPS.read();
  // Serial.print(c);

  loops++;

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another

    nme++;

    lcd1( gpsFix() );
    lcd2( gpsLat() );
    lcd3( gpsLon() );
    lcd4( gpsSpeed() );
    sprintf( buff5, "%06d %06d", loops, nme);
    lcd4( buff5 );
    updateLcd();
  }

 
  t1 = millis();
  Serial.printf( "%s | %s | %s | %08d | %08d | %08d \n", gpsLat(), gpsLon(), gpsSpeed(), loops, nme, t1 - t0 );
  t0 = t1;
}

char buff1[100];

char* gpsFix(void) {
  sprintf( buff1, "fix: %1d (%1d) sat:%02d", (int)GPS.fix, (int)GPS.fixquality, GPS.satellites );
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
