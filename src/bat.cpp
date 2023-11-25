#include "bat.h"
#include "pinout.h"

int batLog(char * log) {
  TRACE("batLog");
  return sprintf( log, "VB,%04,", analogReadMilliVolts( VBAT ) );
}
