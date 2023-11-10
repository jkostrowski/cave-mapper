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

#define LOOP_SLEEP 200
#define QUEUE_SIZE (1000/LOOP_SLEEP)


char log1[400];
char * getLog(void) {
  DateTime now = rtcTimestamp();
  sprintf( log1, "%4d-%02d-%02d %02d:%02d:%02d %s %s %s"
    , now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()
    , getBat()
    , getImuCalibration()
    , getImu9pof()
    );
  return log1;
}

int queueHead = 0; 
char queue[QUEUE_SIZE][512];

void enqueue( char * msg ) {
  strcpy( queue[queueHead], msg);
  queueHead++;

  if (queueHead == QUEUE_SIZE) {
    queueHead = 0;
    for (int i=0; i<QUEUE_SIZE; i++) {
      saveToSd( queue[i]);
      flushSd();
    }
  }

}

// ==============================================

void setup(void) {
    Serial.begin(115200); delay(20);

    initializeLcd();    
    // initializeOta();
    // initializeImu();
    // initializeRtc();
    // initializeSd();
    initializeGps();
}


void loop(void) {
  // updateOta();

  // lcd1( getImuPosition() );
  // lcd2( getImuCalibration() );
  // lcd3( getRtc() );
  // lcd4( getBat() );

  // enqueue( getLog() );

  handleGps();

  // lcd1( gpsFix() );
  // lcd2( gpsPos() );
  // lcd3( gpsSpeed() );

  // delay(LOOP_SLEEP);
}

