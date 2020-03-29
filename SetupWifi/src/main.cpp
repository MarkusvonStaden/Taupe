#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
 
ESP8266WebServer server(80);

bool testWifi(void) {
  Serial.println("Trying to connect with EEPROM Creds");
  int c = 0;
  while (c < 20) {   //Wait for connection
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("Connected");
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  return false;
}
void handleBody() { 
 
      if (server.hasArg("plain")== false){ //Check if body received
 
            server.send(200, "text/plain", "Body not received");
            return;
 
      }
      String message = server.arg("plain");
      
      server.send(200, "application/json", "{\"devicekey\": \""+WiFi.macAddress()+"\"}");

      StaticJsonDocument<200> doc;
      deserializeJson(doc, message);
      String SSID = doc["SSID"];
      String PW = doc["PW"];

      Serial.println("clearing eeprom");
      for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
      }

      Serial.println("Store SSID:");
      for (unsigned int i = 0; i < SSID.length(); ++i){
          EEPROM.write(i, SSID[i]);
      }

      Serial.println("Store Password:");
      for (unsigned int i = 0; i < PW.length(); ++i){
          EEPROM.write(32 + i, PW[i]);
      }
      EEPROM.commit();
      delay(500);
      Serial.println("Restarting the ESP");
      WiFi.softAPdisconnect (true);
      ESP.reset();
}
void startAP(void){
  WiFi.mode(WIFI_AP);
  WiFi.softAP("SMHome");
  delay(100);
  IPAddress Ip(192, 168, 1, 1);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);
  server.on("/body", handleBody);
  server.begin(); //Start the server
  while(1){
    server.handleClient();
  }
}


void setup() {
  Serial.begin(115200);
  EEPROM.begin(96);
  WiFi.setAutoConnect(false);
  WiFi.disconnect();

  String esid;
  for (int i = 0; i < 32; ++i){
    esid += char(EEPROM.read(i));
  }

  String epass;
  for (int i = 32; i < 96; ++i){
    epass += char(EEPROM.read(i));
  }

  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi()){
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else{
    Serial.println();
    Serial.println("Turning the HotSpot On");
    startAP();
  }
}

void loop() {
}
