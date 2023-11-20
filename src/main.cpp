// #include <LowPower.h>
#include <Arduino.h>

#include "ota.h"
#include "lcd.h"
#include "imu.h"
#include "rtc.h"
#include "sdc.h"
#include "bat.h"
#include "gps.h"

// ==============================================

char log1[SDLOG_SIZE];

char * getLog(void) {
  DateTime now = rtcTimestamp();
  sprintf( log1, "%4d-%02d-%02d,%02d:%02d:%02d,%s,%s,%s,%s"
    , now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()
    , getBat()
    , imuCalibration()
    , imu9pof()
    , getGps()
    );
  return log1;
}

char * getLogMini(void) {
  DateTime now = rtcTimestamp();
  sprintf( log1, "%4d-%02d-%02d,%02d:%02d:%02d,%s,%s,%s"
    , now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()
    , getBat()
    , imuCalibration()
    , imu9pof()
    );
  return log1;
}

// ==============================================

void onNmea(void) {
    char* log = getLogMini();
    Serial.println( log );
    sdSaveTo( log );
}

void onFix(void) {
    char label[]= "FIX";
    lcd2( label );
    rtcSet( gpsNow() );
    Serial.println( rtcTime() );
}

void onFixHighAccuracy(void) {
    char label[]= "FIX & Home Set";
    lcd2( label );
}

// ==============================================

void loopUiRefresh(void) {
  lcd1( rtcTime() );
  lcd3( gpsQuality());
  lcd4( imuCalibration() );
    
  // lcd2( gpsFix());
  // lcd4( getBat() );
  // lcd1( gpsFix() );
  // lcd3( gpsLon() );
  // lcd4( gpsSpeed() );

  lcdUpdate();
}

// ==============================================

void setup(void) {
  Serial.begin(115200); delay(50);

  // otaInitialize();
  lcdInitialize();    
  imuInitialize();
  rtcInitialize();
  sdInitialize();
  gpsInitialize();

  gpsOnNmea( &onNmea );
  gpsOnFix( &onFix );
  gpsOnFixGood( &onFixHighAccuracy );
}

// ==============================================

void loop(void) {
  // otaLoop();
  gpsLoop();
  loopUiRefresh();
  sdFlush();
}

// ==============================================



