#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>

const char* ssid = "MyBerg";
const char* password = "42684136518970286588";
const String Type = "Garage";
const String Version = "0.01";


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('*');
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println();
    Serial.println("Connected");
  }
  t_httpUpdate_return ret = ESPhttpUpdate.update("staden-iot.de", 5000, "/checkforupdate", Type+":"+Version);
  switch(ret) {
    case HTTP_UPDATE_FAILED:
        Serial.println("[update] Update failed.");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("[update] Update ok."); // may not be called since we reboot the ESP
        break;
}}

void loop() {
  // put your main code here, to run repeatedly:
}