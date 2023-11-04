#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "pinout.h"

void initializeSd(void);
void saveToSd(char * log);
void flushSd(void);

