#include "bat.h"
#include "pinout.h"

char msgBat[40];

char * getBat(void) {
  sprintf( msgBat, "|%04d|", analogReadMilliVolts( VBAT ) );
    // analogRead( VBAT ),
  // lcd4( msgBat );
  return msgBat;
}
