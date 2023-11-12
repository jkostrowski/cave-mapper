#include "bat.h"
#include "pinout.h"

char msgBat[40];

char * getBat(void) {
  sprintf( msgBat, "VB,%04d", analogReadMilliVolts( VBAT ) );
    // analogRead( VBAT ),
  return msgBat;
}
