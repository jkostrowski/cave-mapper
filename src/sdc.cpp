#include "sdc.h"

File sd;
SPIClass spi1( HSPI );



void sdInitialize(void) {
  pinMode(SD_CS, OUTPUT); 

  spi1.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  spi1.setDataMode(SPI_MODE0);

  if(!SD.begin(SD_CS, spi1)){
      Serial.println("Card Mount Failed");
      return;
  }
  Serial.println("SD initialization complete.");

  int i = 0;
  char name[] = "/cave00.v2.csv";
  while (SD.exists(name)) {
    Serial.print( name );
    Serial.println( " found.");
    i++;
    name[5] = '0' + i/10;
    name[6] = '0' + i%10;
  }

  sd = SD.open( name, FILE_WRITE );

  Serial.println(sd ? "File is OK": "File error");
}


// =================================

#define QUEUE_SIZE 10

int queueHead = 0; 
char queue[QUEUE_SIZE][SDLOG_SIZE];

void sdQueue(char* log) {
  
  strcpy( queue[queueHead], log);
  queueHead++;

  if (queueHead == QUEUE_SIZE) {
    queueHead = 0;
    for (int i=0; i<QUEUE_SIZE; i++) {
      sd.println(queue[i]);
    }
    sd.flush();
  }

}

// =================================

