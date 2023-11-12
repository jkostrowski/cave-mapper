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
    , getImuCalibration()
    , getImu9pof()
    , getGps()
    );
  return log1;
}


// ==============================================

void loopUiRefresh(void) {
  lcd1( getImuPosition() );
  lcd2( getImuCalibration() );
  lcd3( getRtc() );
  lcd4( gpsLat() );

  // lcd4( getBat() );
  // lcd1( gpsFix() );
  // lcd3( gpsLon() );
  // lcd4( gpsSpeed() );

  updateLcd();
}

// ==============================================

void setup(void) {
    Serial.begin(115200); delay(50);

    // otaInitialize();
    initializeLcd();    
    initializeImu();
    initializeRtc();
    sdInitialize();
    initializeGps();
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



