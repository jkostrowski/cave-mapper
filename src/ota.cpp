#include "ota.h"


void otaInitialize(void) {
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


void otaLoop(void ) {
    ArduinoOTA.handle();
}

