#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "pinout.h"

#define SDLOG_SIZE 512

void sdInitialize(void);
void sdSaveTo(char * log);
void sdQueue(char * log);
void sdFlush(void);

