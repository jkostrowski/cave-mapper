#include "imu.h"

// Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire); 
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire1);
char msgImu1[100];
char msgImu2[100];
char msgImu3[100];

void initializeImu(void) {
    
    if (!Wire1.begin(SDA2, SCL2)) {
        Serial.println("Problem with Wire1");     
        // lcd1("Wire1 error.");
    }

    if(!bno.begin())
    {
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    } 

    Serial.println("IMU Orientation Sensor OK."); 
    bno.setExtCrystalUse(true);
}

char* getImuCalibration(void) {
  uint8_t sys, gyro, accel, mag;
  sys = gyro = accel = mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  sprintf( msgImu1, "|%1d|%1d|%1d|%1d|", sys, gyro, accel, mag );
  return msgImu1;
}

char* getImuPosition(void) {
  sensors_event_t event; 
  bno.getEvent(&event);

  sprintf(msgImu2, "X: %.1f Y: %.1f Z: %.1f\n", event.orientation.x, event.orientation.y, event.orientation.z );
  return msgImu2;
}

char* getImu9pof(void) {
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData  , Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData  , Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData  , Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData , Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData      , Adafruit_BNO055::VECTOR_GRAVITY);

  sprintf(msgImu3,"|%3.0f,%3.0f,%3.0f|%3.0f,%3.0f,%3.0f|%3.0f,%3.0f,%3.0f|%3.0f,%3.0f,%3.0f|%3.0f,%3.0f,%3.0f|%3.0f,%3.0f,%3.0f|%02d|", 
    orientationData.orientation.x,  orientationData.orientation.y, orientationData.orientation.z,
    angVelocityData.gyro.x, angVelocityData.gyro.y, angVelocityData.gyro.z, 
    linearAccelData.acceleration.x, linearAccelData.acceleration.y, linearAccelData.acceleration.z,
    magnetometerData.magnetic.x, magnetometerData.magnetic.y, magnetometerData.magnetic.z,
    accelerometerData.acceleration.x,accelerometerData.acceleration.y,accelerometerData.acceleration.z,
    gravityData.acceleration.x,gravityData.acceleration.y,gravityData.acceleration.z,
    bno.getTemp());  

   return( msgImu3 );
}



void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displaySensorStatus(void) {
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}


// char queue[5][300];
// int i = 0;
// i++;
// if (i==5) {
// // if (file) {
// //   saveToSd(queue[0]);
// //   saveToSd(queue[1]);
// //   saveToSd(queue[2]);
// //   saveToSd(queue[3]);
// //   saveToSd(queue[4]);
// //   flushSd();
// // } 
// i = 0;