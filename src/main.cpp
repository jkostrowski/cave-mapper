#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SD.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE,  SCL, SDA);   

// ==============================================

void displaySensorDetails(void);
void displaySensorStatus(void);
void displayCalStatus(void);
void displayPosition(void);
void displayAll(void);
void displayAll2(void);
void displayLcd(void);

void initializeSd(void);
void initializeImu(void);

void initializeLcd(void);

// ==============================================


void setup(void) 
{
  Serial.begin(57600); delay(10);
  
  initializeSd();
  initializeImu();
  initializeLcd();

  delay(1000);

  //displaySensorDetails();
  //displaySensorStatus();
}

void loop(void) 
{
  // displayPosition();
  // displayCalStatus();

  // displayAll2();
  displayLcd();

  delay(50);
}


// ==============================================

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire);

void lcd( const char* msg ) {
  u8g2.clearBuffer();					        
  // u8g2.setFont(u8g2_font_ncenB14_tr);	
  u8g2.setFont(u8g2_font_fub11_tr);	
  u8g2.drawStr(0,20, msg );	
  u8g2.sendBuffer();					        
}


void initializeLcd(void) {
    u8g2.begin();
    lcd( "Starting..." );
}

// ==============================================

File file;

void initializeSd(void) {

  Serial.print("Initializing SD card...");
  pinMode(D8, OUTPUT);  // CS
 
  if (!SD.begin(D8)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialization done.");

  int i = 0;
  char name[] = "cave00.csv";
  while (SD.exists(name)) {
    Serial.print( name );
    Serial.println( " found.");
    i++;
    name[4] = '0' + i/10;
    name[5] = '0' + i%10;
  }

  file = SD.open( name, FILE_WRITE );
 
  if (file) {
    file.println();
    Serial.println("File is OK");
  } else {
    Serial.println("File error");
  }
}

void saveToSd( char* msg ) {
  file.println( msg );
}

void flushSd(void) {
   file.flush();
}

// ==============================================

void initializeImu(void) {
  Serial.println("IMU Orientation Sensor Test"); Serial.println("");

  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  } 

  bno.setExtCrystalUse(true);
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

void displaySensorStatus(void)
{
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

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */

  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
  Serial.print(" | ");
  
}

void displayPosition(void)
{
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 2);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 2);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 2);
  Serial.println("");
}

char buffer[100];

void displayLcd(void) {

  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  uint8_t csys, cgyro, cacc, cmag = 0;
  bno.getCalibration(&csys, &cgyro, &cacc, &cmag);


  sprintf(buffer,"%3.0f,%3.0f,%3.0f|%1d|%1d|%1d|%1d", 
    orientationData.orientation.x,  orientationData.orientation.y, orientationData.orientation.z,
    // angVelocityData.gyro.x, angVelocityData.gyro.y, angVelocityData.gyro.z, 
    // linearAccelData.acceleration.x, linearAccelData.acceleration.y, linearAccelData.acceleration.z,
    // magnetometerData.magnetic.x, magnetometerData.magnetic.y, magnetometerData.magnetic.z,
    // accelerometerData.acceleration.x,accelerometerData.acceleration.y,accelerometerData.acceleration.z,
    // gravityData.acceleration.x,gravityData.acceleration.y,gravityData.acceleration.z,
    // bno.getTemp(), 
    csys, cgyro, cacc, cmag
    );  

  lcd( buffer );
}

char queue[4][300];
int i = 0;

void displayAll2(void) {
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  uint8_t csys, cgyro, cacc, cmag = 0;
  bno.getCalibration(&csys, &cgyro, &cacc, &cmag);


  sprintf(queue[i],"O,%.2f,%.2f,%.2f,G,%.2f,%.2f,%.2f,L,%.2f,%.2f,%.2f,M,%.2f,%.2f,%.2f,L,%.2f,%.2f,%.2f,G,%.2f,%.2f,%.2f,T,%d,CS,%d,CG,%d,CA,%d,CM,%d", 
    orientationData.orientation.x,  orientationData.orientation.y, orientationData.orientation.z,
    angVelocityData.gyro.x, angVelocityData.gyro.y, angVelocityData.gyro.z, 
    linearAccelData.acceleration.x, linearAccelData.acceleration.y, linearAccelData.acceleration.z,
    magnetometerData.magnetic.x, magnetometerData.magnetic.y, magnetometerData.magnetic.z,
    accelerometerData.acceleration.x,accelerometerData.acceleration.y,accelerometerData.acceleration.z,
    gravityData.acceleration.x,gravityData.acceleration.y,gravityData.acceleration.z,
    bno.getTemp(), csys, cgyro, cacc, cmag
    );  

  Serial.println(queue[i]); 
  i++;

  if (i==4) {
    if (file) {
      saveToSd(queue[0]);
      saveToSd(queue[1]);
      saveToSd(queue[2]);
      saveToSd(queue[3]);
      flushSd();
    } 
    i = 0;
  }

}

void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("A,");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("O,");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    Serial.print("M,");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    Serial.print("G,");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    Serial.print("R,");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    Serial.print("L,");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    Serial.print("G,");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    Serial.print("?,");
  }

  Serial.print(x);
  Serial.print(","); 
  Serial.print(y); 
  Serial.print(",");  
  Serial.print(z);
  Serial.print(",");  
}

void displayAll(void)
{
  //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  printEvent(&orientationData);
  printEvent(&angVelocityData);
  printEvent(&linearAccelData);
  printEvent(&magnetometerData);
  printEvent(&accelerometerData);
  printEvent(&gravityData);

  int8_t boardTemp = bno.getTemp();
  Serial.print(F(" T: "));
  Serial.print(boardTemp);

  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  Serial.print(" CS=");
  Serial.print(system);
  Serial.print(" CG=");
  Serial.print(gyro);
  Serial.print(" CA=");
  Serial.print(accel);
  Serial.print(" CM=");
  Serial.print(mag);

  Serial.println("");
}

