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


// ==============================================

void loopUiRefresh(void) {
  lcd1( rtcTime() );
  lcd2( gpsFix());
  lcd3( gpsQuality());
  lcd4( imuCalibration() );
    
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
}

// ==============================================


void loop(void) {
  // otaLoop();

  for (int i=0; i<100; i++) {
    if (gpsSinglePass()) {
        char* log = getLog();
        Serial.println( log );
        sdSaveTo( log );
    }
  }

  loopUiRefresh();
  sdFlush();
}



