#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

const String Type = "Garage";
const String Version = "0.01";

String SSID;
String PASSWORD;

ESP8266WebServer server(80);
HTTPClient http;

void debug(String message){
  digitalWrite(LED_BUILTIN, 0);
  http.begin("http://staden-iot.de:5000/debug");
  http.addHeader("Content-Type", "text/plain");
  http.POST(message);
  http.end();
  delay(100);
  digitalWrite(LED_BUILTIN, 1);
}

void ReadEEPROM(void){
  SSID = "";
  for (int i = 0; i < 32; ++i){
    SSID += char(EEPROM.read(i));
  }

  PASSWORD = "";
  for (int i = 32; i < 96; ++i){
    PASSWORD += char(EEPROM.read(i));
  }
}

bool ConnectToSavedWiFi(void){
  WiFi.begin(SSID.c_str(), PASSWORD.c_str());
  int count = 0;
  while(count < 20){
    if (WiFi.status() == WL_CONNECTED){
      return true;
    }
    delay(500);
    count++;
  }
  return false;
} 

void handleBody(void){
  if (server.hasArg("plain")== false){
 
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
  server.begin();
  while(1){
    server.handleClient();
  }
}

bool update(void){
  return (HTTP_UPDATE_FAILED != ESPhttpUpdate.update("staden-iot.de", 5000, "/checkforupdate", Type+":"+Version));
}

void setup() {
  WiFi.setAutoConnect(false);
  WiFi.disconnect();
  EEPROM.begin(96);

  ReadEEPROM();
  if (ConnectToSavedWiFi() == false){
    startAP();
  }

  update();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  delay(10000); 
  debug("Pimmel123");
}