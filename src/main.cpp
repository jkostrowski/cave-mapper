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
  char* log = (char *) &log1;
  int i = 0;

  // Serial.printf("rtc i=%i\n",i);
  i = rtcLog(log); 
  log += i;
  i = batLog( log);
  log += i;
  i = imuCalibrationLog( log);
  log += i;
  i = imuPositionLog( log);
  log += i;
  i = gpsLog( log);

  return log1;
}

// ==============================================

void onNmea(void) {
    char* log = getLog();
    Serial.println( log );
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
  lcd2( imuCalibrationLog() );
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
  timerAlarmWrite(timer, 500*1000, true);
  timerAlarmEnable(timer);  
}


// ==============================================
#undef GPS
#define GPS 

typedef struct {
  int16_t a;
  int16_t b;
  int16_t c; 
} tuple_t;


void setup(void) {
  Serial.begin(115200); delay(50);

  lcdInitialize();    
  imuInitialize();
  rtcInitialize();
  sdInitialize();

#ifdef GPS
  gpsInitialize();
  gpsOnNmea( &onNmea );
  gpsOnFix( &onFix );
  gpsOnFixGood( &onFixHighAccuracy );
#endif

  imuCalibrate();
  isrInitialize();
}

// ==============================================

void loop(void) {

#ifdef GPS
    gpsLoop();
#endif

#ifndef GPS
    delay(10);
#endif

  if (updateUi) {
    loopUiRefresh();
    updateUi = false;
  }
}

// ==============================================



