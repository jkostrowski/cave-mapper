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

Ticker tickerUiUpdate;
Ticker tickerGpsRead;
Ticker tickerGpsParse;

// ==============================================

char log1[SDLOG_SIZE];

char * getLog(void) {
  char* log = (char *) &log1;
  int i = 0;

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
    char label[]= "F_";
    lcd1r( label );
    rtcSet( gpsNow() );
}

void onFixHighAccuracy(void) {
    char label[]= "FH";
    lcd1r( label );
}

// ==============================================

void loopUiRefresh(void) {

  lcd1( rtcTimeLog() );
  lcd2( imuCalibrationUi() );
  lcd3( gpsQuality());
  lcd4( imuPositionUi());
   
  lcdUpdate();
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

  imuCalibrate();

#ifdef GPS
  gpsInitialize();
  gpsOnNmea( &onNmea );
  gpsOnFix( &onFix );
  gpsOnFixGood( &onFixHighAccuracy );

  tickerGpsRead.attach_ms(    1, gpsRead );
  tickerGpsParse.attach_ms(  20, gpsParse );
#endif

  tickerUiUpdate.attach_ms( 500, loopUiRefresh );
}

// ==============================================

void loop(void) {}

// ==============================================
