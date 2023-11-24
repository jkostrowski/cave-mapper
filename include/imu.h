#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "pinout.h"
#include "sdc.h"


#define IMUDAT "/imu.dat"

void  imuInitialize(void);
char* imuPosition(void);
char* imu9pof(void);
char* imuCalibrationLog(void);
void  imuCalibrate(void);

sensors_event_t imuPositionEvent(void);

// void imuGetCalibration(adafruit_bno055_offsets_t &so );

// void displaySensorDetails(void);
// void displaySensorStatus(void);
// void displayCalStatus(void);
// void displayPosition(void);
// void displayImu(void);
// void saveImuData(void);
