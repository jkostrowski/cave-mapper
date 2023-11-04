#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "pinout.h"

void initializeImu(void);
char* getImuPosition(void);
char* getImu9pof(void);

char* getImuCalibration(void);


// void displaySensorDetails(void);
// void displaySensorStatus(void);
// void displayCalStatus(void);
// void displayPosition(void);
// void displayImu(void);
// void saveImuData(void);
