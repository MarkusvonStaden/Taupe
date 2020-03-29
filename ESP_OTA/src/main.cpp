#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>

//#define Version  0.01
//#define updateURL "http://staden-iot.de/updatecheck.json"

const String SSID = "MyBerg";
const String Password = "42684136518970286588"; 
const String Type = "GarageDoor:";
const String Version = "0.0001";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID,Password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("*");
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println();
    Serial.println("Sucess");
  }
  // put your setup code here, to run once:
  //Serial.println(Version);
  t_httpUpdate_return ret = ESPhttpUpdate.update("192.168.178.151", 8000, "/checkforupdate", Type+Version);
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
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}