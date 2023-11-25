#include <Arduino.h>
#include <Ticker.h>

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


char log1[SDLOG_SIZE];

char * getLog(void) {
  char* log = (char *) &log1;
  int i = 0;

  // Serial.printf("rtc i=%i\n",i);
  i = rtcFullLog(log); 
  log += i;
  i = batLog(log);
  log += i;
  i = imuCalibrationLog(log);
  log += i;
  i = imuPositionLog(log);
  log += i;
  i = gpsLog(log);

  return log1;
}

// ==============================================

void onNmea(void) {
    char* log = getLog();
    Serial.println( log );
    sdQueue( log );
}

void onFix(void) {
    char label[]= "F-";
    lcd1r( label );
    rtcSet( gpsNow() );
    Serial.println( rtcTimeLog() );
}

void onFixHighAccuracy(void) {
    char label[]= "FH";
    lcd1r( label );
}

// ==============================================

char buff7[100];

void loopUiRefresh(void) {

  lcd1( rtcTimeLog() );
//  lcd1r( gpsFixUi() );
  lcd2( imuCalibrationUi() );
  lcd3( gpsQuality());
  lcd4( imuPositionUi());
   
  lcdUpdate();
}


// ==============================================

Ticker tickerUiUpdate;
Ticker tickerGpsRead;
Ticker tickerGpsParse;

volatile bool gpsReadFlag  = false;
volatile bool gpsParseFlag = false;
volatile bool uiUpdateFlag = false;

// hw_timer_t * timerGpsRead = NULL;
// portMUX_TYPE timerGpsReadMux = portMUX_INITIALIZER_UNLOCKED;

// hw_timer_t * timerGpsParse = NULL;
// portMUX_TYPE timerGpsParseMux = portMUX_INITIALIZER_UNLOCKED;

// hw_timer_t * timerUiUpdate = NULL;
// portMUX_TYPE timerUiUpdateMux = portMUX_INITIALIZER_UNLOCKED;

void onTimerGpsRead() {
  // portENTER_CRITICAL_ISR(&timerGpsReadMux);
  gpsReadFlag = true;
  // portEXIT_CRITICAL_ISR(&timerGpsReadMux);
}

void onTimerGpsParse() {
  // portENTER_CRITICAL_ISR(&timerGpsParseMux);
  gpsParseFlag = true;
  // portEXIT_CRITICAL_ISR(&timerGpsParseMux);
}

void onTimerUiUpdate() {
  // portENTER_CRITICAL_ISR(&timerUiUpdateMux);
  uiUpdateFlag = true;
  // portEXIT_CRITICAL_ISR(&timerUiUpdateMux);
}


void isrInitialize(void) {
  tickerGpsRead.attach_ms( 1, onTimerGpsRead );
  tickerGpsParse.attach_ms( 25, onTimerGpsParse );
  tickerUiUpdate.attach_ms( 500, onTimerUiUpdate );

  // timerGpsRead = timerBegin(0, 80, true);
  // timerAttachInterrupt(timerGpsRead, &onTimerGpsRead, true);
  // timerAlarmWrite(timerGpsRead, 1*1000, true);
  // timerAlarmEnable(timerGpsRead);  

  // timerGpsParse = timerBegin(0, 80, true);
  // timerAttachInterrupt(timerGpsParse, &onTimerGpsParse, true);
  // timerAlarmWrite(timerGpsParse, 25*1000, true);
  // timerAlarmEnable(timerGpsParse);  

  // timerUiUpdate = timerBegin(0, 80, true);
  // timerAttachInterrupt(timerUiUpdate, &onTimerUiUpdate, true);
  // timerAlarmWrite(timerUiUpdate, 500*1000, true);
  // timerAlarmEnable(timerUiUpdate);  
}


// ==============================================
#undef GPS
#define GPS 


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

  // imuCalibrate();
  isrInitialize();
}

// ==============================================

void loop(void) {

#ifdef GPS
  if (gpsReadFlag) {
    gpsRead();
    gpsReadFlag = false;
  }

  if (gpsParseFlag) {
    gpsParse();
    gpsParseFlag = false;
  }
#endif

  if (uiUpdateFlag) {
    loopUiRefresh();
    uiUpdateFlag = false;
  }
}

// ==============================================



/*

imu offsets saved
[ 99682][E][esp32-hal-cpu.c:110] addApbChangeCallback(): duplicate func=0x4200db38 arg=0x3fc92994
E (99552) timer_group: timer_isr_callback_add(236): register interrupt service failed
[ 99690][E][esp32-hal-cpu.c:110] addApbChangeCallback(): duplicate func=0x4200db38 arg=0x3fc92994
E (99568) timer_group: timer_isr_callback_add(236): register interrupt service failed



*/

