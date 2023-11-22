// #include <LowPower.h>
#include <Arduino.h>

// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>

#include "ota.h"
#include "lcd.h"
#include "imu.h"
#include "rtc.h"
#include "sdc.h"
#include "bat.h"
#include "gps.h"

// ==============================================

DateTime reboot;

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
    char* log = getLog();
//    char* log = getLogMini();
    Serial.println( log );
    // sdSaveTo( log );
    sdQueue( log );
}

void onFix(void) {
    char label[]= "FIX";
    lcd4( label );
    rtcSet( gpsNow() );
    Serial.println( rtcTime() );
}

void onFixHighAccuracy(void) {
    char label[]= "FIX & Home Set";
    lcd4( label );
}

// ==============================================

char buff7[100];

void loopUiRefresh(void) {

  lcd1( rtcTime() );
  lcd2( imuCalibration() );
  lcd3( gpsQuality());
   
  lcdUpdate();
}


// ==============================================

volatile bool updateUi = false;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  updateUi = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void isrInitialize(void) {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100*1000, true);
  timerAlarmEnable(timer);  
}


// ==============================================

void setup(void) {
  Serial.begin(115200); delay(50);

  lcdInitialize();    
  imuInitialize();
  rtcInitialize();
  sdInitialize();

  gpsInitialize();
  gpsOnNmea( &onNmea );
  gpsOnFix( &onFix );
  gpsOnFixGood( &onFixHighAccuracy );

  isrInitialize();
}

// ==============================================

void loop(void) {
  gpsLoop();
  if (updateUi) {
    loopUiRefresh();
    updateUi = false;
  }
}

// ==============================================



// void setup(void) {
//   Serial.begin(115200); delay(50);
//   imuInitialize();
// }
//
// void loop(void) {
// }

