
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "pinout.h"

#define SDLOG_SIZE 512


void sdInitialize(void);
void sdQueue(char * log);

