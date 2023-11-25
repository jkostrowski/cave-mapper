#include "imu.h"

#include <EEPROM.h>
#include <SD.h>

#define BNO055_SAMPLERATE_DELAY_MS  200
#define BNO055_EEPROM               0


Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire1);
char msgImu1[50];
char msgImu2[50];
char msgImu3[200];

void imuInitialize(void) {
    if (!Wire1.begin(SDA2, SCL2)) {
        Serial.println("Problem with Wire1");     
    }

    if(!bno.begin()) {
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    } 

    Serial.println("IMU Orientation Sensor OK."); 
    bno.setMode( OPERATION_MODE_NDOF );
    bno.setExtCrystalUse(true);
}

// ======================================================================================

int imuPositionLog(char* log) {
  TRACE( "imuPositionLog" );
  
  // imu::Quaternion quat = bno.getQuat(); // TODO

  sensors_event_t  orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData  , Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData  , Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData  , Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData , Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData      , Adafruit_BNO055::VECTOR_GRAVITY);

  return sprintf(log,"EU,% 7.2f,% 7.2f,% 7.2f,GY,% 7.2f,% 7.2f,% 7.2f,LA,% 7.2f,% 7.2f,% 7.2f,MA,% 7.2f,% 7.2f,% 7.2f,AC,% 7.2f,% 7.2f,% 7.2f,GR,% 7.2f,% 7.2f,% 7.2f,T,%02d", 
    orientationData.orientation.x,  orientationData.orientation.y, orientationData.orientation.z,
    angVelocityData.gyro.x, angVelocityData.gyro.y, angVelocityData.gyro.z, 
    linearAccelData.acceleration.x, linearAccelData.acceleration.y, linearAccelData.acceleration.z,
    magnetometerData.magnetic.x, magnetometerData.magnetic.y, magnetometerData.magnetic.z,
    accelerometerData.acceleration.x,accelerometerData.acceleration.y,accelerometerData.acceleration.z,
    gravityData.acceleration.x,gravityData.acceleration.y,gravityData.acceleration.z,
    bno.getTemp());  
}

char* imuPositionUi(void) {
  TRACE("imuHeadinglog");
  sensors_event_t event; 
  bno.getEvent(&event);

  sprintf(msgImu2, "IMU:% 7.2f", event.orientation.x );
  return msgImu2;
}

// ======================================================================================

int imuCalibrationLog(char* log) {
  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  return sprintf( log, "s,%1d,g,%1d,a,%1d,m,%1d,", sys, gyro, accel, mag );
}

char* imuCalibrationUi(void) {
  imuCalibrationLog( msgImu1 );
  return msgImu1;
}

// ======================================================================================

void displaySensorDetails(void) {
    sensor_t sensor;
    bno.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print("Sensor:       "); Serial.println(sensor.name);
    Serial.print("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
    Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
    Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);
}

void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData) {
    Serial.print("Acc: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");


    Serial.print("\nGyr: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");


    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
    Serial.print("\n");
}

// ======================================================================================

size_t saveImu(adafruit_bno055_offsets_t &data) {
  File f = SD.open( IMUDAT, FILE_WRITE );
  size_t s =  sizeof(data)/sizeof(char);
  size_t x = f.write( (uint8_t *) &data, s );
  f.close();

  Serial.printf( "imu saved: %d of %d\n", x, s );
  return x;
}

size_t loadImu(adafruit_bno055_offsets_t &data) {
  if (!SD.exists(IMUDAT)) 
    return 0;

  File   f = SD.open( IMUDAT, FILE_READ );
  size_t s = sizeof(data)/sizeof(char);
  size_t x = f.readBytes((char *) &data, s );
  f.close();
  
  Serial.printf( "imu loaded: %d of %d\n", x, s );
  return x;
}

// ======================================================================================

void imuCalibrate(void) {
  
  adafruit_bno055_offsets_t o = {};

  // Serial.println("imu load offsets"); 
  // if (!loadImu(o)) {
  //   o =  { -37, -66, 2, -1, -4, 0, -351, 474, 170, 1000, 455 };
  // }

  // Serial.println("imu bootstrap offsets"); 
  // displaySensorOffsets(o);
  // bno.setSensorOffsets(o);
  // delay(1000);
  // displaySensorDetails();
  // bno.setExtCrystalUse(true);

  Serial.println("imu re-calibrate"); 
  while (!bno.isFullyCalibrated()) {
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.println( imuCalibrationUi() );
    delay(50);
  }

  Serial.println("imu calibrated"); 

  adafruit_bno055_offsets_t p = {};

  if (bno.getSensorOffsets(p)) {
    displaySensorOffsets(o);
    if (saveImu(o))
      Serial.println("imu offsets saved");
    else 
      Serial.println("imu offsets not saved - sd error");
  } else {
    Serial.println("imu offsets not saved - callibration error");
  }
}

void imuCalibrateWithEeprom(void) {

    // double degToRad = 57.295779513;
    // int eeAddress = BNO055_EEPROM;
    // long id;
    // bool foundCalib = false;
    // sensor_t sensor;
    // adafruit_bno055_offsets_t calibrationData;

    // if (!EEPROM.begin(512)) {
    //   Serial.println("EEPROM failed to initialise");
    //   while (true);
    // }
    // else {
    //   Serial.println("EEPROM initialised");

    //   byte inByte = EEPROM.read(0);
    //   Serial.print("read ");
    //   Serial.println(inByte);  

    //   EEPROM.write(0, inByte + 1);
    //   EEPROM.commit();

    //   byte inByte2 = EEPROM.read(0);
    //   Serial.print("read ");
    //   Serial.println(inByte2);  
    // }

    // check sensor's unique ID at the beginning oF EEPROM.
    // bno.getSensor(&sensor);
    // EEPROM.get(eeAddress, id);

    // if (id != sensor.sensor_id) {
    //     Serial.println("No Calibration Data for this sensor exists in EEPROM");
    // }
    // else {
    //   Serial.println("Found Calibration for this sensor in EEPROM.");
    //   EEPROM.get(eeAddress + sizeof(long), calibrationData);
    //   displaySensorOffsets(calibrationData);

    //   Serial.println("Restoring Calibration data to the BNO055...");
    //   bno.setSensorOffsets(calibrationData);
  
    //   Serial.println("Calibration data loaded into BNO055");
    //   foundCalib = true;
    // }


      // Serial.println("Storing calibration data to EEPROM...");
      // eeAddress = BNO055_EEPROM;
      // EEPROM.put(eeAddress, id);
      // eeAddress += sizeof(long);
      // EEPROM.put(eeAddress, newCalib);
      // EEPROM.commit();
      // Serial.println("Data stored to EEPROM.");

}

// sensors_event_t imuPositionEvent(void) {
//   sensors_event_t event; 
//   bno.getEvent(&event);
//   return event;
// }
