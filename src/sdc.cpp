#include "sdc.h"


File sd;
SPIClass spi1( HSPI );


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
