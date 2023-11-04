#include <Arduino.h>
#include <ArduinoOTA.h>
#include <U8g2lib.h>
#include <WiFi.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include <SD.h>
#include <RTClib.h>

#include "secrets.h"

// I2C 

#define SDA1 17  // oled
#define SCL1 18  // oled
#define RST  21  // oled

#define SDA2 33  // BNO and RTC
#define SCL2 34  // BNO and RTC

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, RST, SCL1, SDA1); // screen on heltec

// U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE,  SCL, SDA); - mini external screen 


// ==============================================


void initializeOta(void);
void updateOta(void );

void initializeLcd(void);
void updateLcd(void);

void initializeImu(void);
void showImu(void);
void showCalibration(void);

void initializeRtc(void);
void showRtc(void);
DateTime rtcTimestamp(void);

void showBat(void);
int getBat(void);

void initializeSd(void);

void updateLog(void);

// void displaySensorDetails(void);
// void displaySensorStatus(void);
// void displayCalStatus(void);
// void displayPosition(void);
// void displayImu(void);
// void saveImuData(void);

// void initializeSd(void);


// ==============================================

void setup(void) 
{
    Serial.begin(115200); delay(20);

    initializeLcd();    
    initializeOta();
    initializeImu();
    initializeRtc();
    initializeSd();
}

void loop(void) 
{
  updateOta();

  // displayPosition();
  // displayCalStatus();
  // displayLcd();

  // displayCalStatus();
  // saveImuData();

  showImu();
  showCalibration();
  showRtc();
  showBat();

  updateLcd();
  updateLog();

  delay(250);

}

// ==============================================

File sd;
SPIClass spi1( HSPI );

#define SD_CS   7          // good: 5, 7,  26 
#define SD_MOSI 6          // good: 4, 21 (CONFLICT)
#define SD_MISO 5          // good: 3, 19
#define SD_SCLK 4          // good: 2, 20


void initializeSd(void) {
  pinMode(SD_CS, OUTPUT); 

  spi1.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  spi1.setDataMode(SPI_MODE0);

  if(!SD.begin(SD_CS, spi1)){
      Serial.println("Card Mount Failed");
      return;
  }
  Serial.println("SD initialization complete.");

  int i = 0;
  char name[] = "/cave00.csv";
  while (SD.exists(name)) {
    Serial.print( name );
    Serial.println( " found.");
    i++;
    name[5] = '0' + i/10;
    name[6] = '0' + i%10;
  }

  sd = SD.open( name, FILE_WRITE );

  if (sd) {
    sd.println("aaaaa");
    sd.flush();
    Serial.println("File is OK");
  } else {
    Serial.println("File error");
  }
}

void saveToSd( char* msg ) {
  sd.println( msg );
}

void flushSd(void) {
  sd.flush();
}



// ==============================================

void initializeLcd(void) {
    u8g2.begin();
    u8g2.clearBuffer();					        
    u8g2.setFont(u8g2_font_fub11_tr);	
  //  u8g2.setFont(u8g2_font_profont10_tf);
    u8g2.setFontPosTop();
    u8g2.drawStr(10,10, "Ello!");	
    u8g2.sendBuffer();					        
}

char l1[100] = {0};
char l2[100] = {0};
char l3[100] = {0};
char l4[100] = {0};

void lcd1(char * msg) {
  strcpy( l1, msg );
}

void lcd2(char * msg) {
  strcpy( l2, msg );
}

void lcd3(char * msg) {
  strcpy( l3, msg );
}

void lcd4(char * msg) {
  strcpy( l4, msg );
}

void updateLcd() {
    u8g2.clearBuffer();
    u8g2.drawStr(0, 0, l1);	
    u8g2.drawStr(0,15, l2);	
    u8g2.drawStr(0,30, l3);	
    u8g2.drawStr(0,45, l4);	
    u8g2.sendBuffer();					        
}

// ==============================================

void initializeOta(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  ArduinoOTA.setHostname( "CaveMapper32");
  ArduinoOTA.begin(); 
 
  delay(10);

}

void updateOta(void ) {
    ArduinoOTA.handle();
}



// ==============================================================

RTC_DS3231 rtc;

void initializeRtc(void) {
    if (! rtc.begin(&Wire1)) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void showRtc(void) {
    DateTime now = rtc.now();
    char msg[100];
    sprintf( msg, "%02d:%02d:%02d", now.hour(), now.minute(), now.second()  );
    lcd3( msg );
}

DateTime rtcTimestamp(void) {
    return rtc.now();
}



// ==============================================

int getBat(void) {
  return analogReadMilliVolts(1);
}

void showBat(void) {
  char msg[20];
  sprintf( msg, "%4d | %4d",  analogRead(1), analogReadMilliVolts(1) );
  lcd4( msg );
}

// ==============================================

// Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire);   // external lcd
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29, &Wire1);

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


void showImu(void) {
  sensors_event_t event; 
  bno.getEvent(&event);

  char msg[100];

  sprintf(msg, "X: %.1f Y: %.1f Z: %.1f\n", event.orientation.x, event.orientation.y, event.orientation.z );
  Serial.print( msg );
  lcd1( msg );
}

void showCalibration(void)
{
  uint8_t sys, gyro, accel, mag;
  sys = gyro = accel = mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  char buff[20];
  sprintf( buff, "S%1d|G%1d|A%1d|M%1d", sys, gyro, accel, mag );

  lcd2( buff );
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


void displayPosition(void) {
  sensors_event_t event; 
  bno.getEvent(&event);
  
  Serial.print("\tX: "); Serial.print(event.orientation.x, 2);
  Serial.print("\tY: "); Serial.print(event.orientation.y, 2);
  Serial.print("\tZ: "); Serial.print(event.orientation.z, 2);
  Serial.println("");
}

void displayImu(void) {
  char buffer[100];

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

  lcd1( buffer );
}

char queue[5][300];
int i = 0;

void saveImuData(void) {
  sensors_event_t orientation, gyro, linaccel, magn, accel, gravity;
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linaccel, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magn, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravity, Adafruit_BNO055::VECTOR_GRAVITY);

  uint8_t csys, cgyro, cacc, cmag = 0;
  bno.getCalibration(&csys, &cgyro, &cacc, &cmag);


  sprintf(queue[i],"O,%.1f,%.1f,%.1f,GY,%.1f,%.1f,%.1f,LA,%.1f,%.1f,%.1f,AC,%.1f,%.1f,%.1f,MA,%.1f,%.1f,%.1f,GR,%.1f,%.1f,%.1f,T,%d,CS,%d,CG,%d,CA,%d,CM,%d", 
    orientation.orientation.x,  orientation.orientation.y, orientation.orientation.z,
    gyro.gyro.x, gyro.gyro.y, gyro.gyro.z, 
    linaccel.acceleration.x, linaccel.acceleration.y, linaccel.acceleration.z,
    accel.acceleration.x,accel.acceleration.y,accel.acceleration.z,
    magn.magnetic.x, magn.magnetic.y, magn.magnetic.z,
    gravity.acceleration.x,gravity.acceleration.y,gravity.acceleration.z,
    bno.getTemp(), 
    csys, cgyro, cacc, cmag
    );  

  Serial.println(queue[i]); 
  i++;

  if (i==5) {
    // if (file) {
    //   saveToSd(queue[0]);
    //   saveToSd(queue[1]);
    //   saveToSd(queue[2]);
    //   saveToSd(queue[3]);
    //   saveToSd(queue[4]);
    //   flushSd();
    // } 
    i = 0;
  }
}

// ==============================================

int logPass = 0; 

void updateLog(void) {
  logPass++;
  if (logPass == 9*4) {
    logPass = 0;
    DateTime now = rtcTimestamp();
    char msg[100];
    sprintf( msg, "%02d:%02d:%02d Bat:%4d", now.hour(), now.minute(), now.second(), getBat() );
    saveToSd( msg );
    flushSd();
  }
}
