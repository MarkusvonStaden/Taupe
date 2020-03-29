#include <Arduino.h>
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  EEPROM.begin(96);

  String esid;
        for (int i = 0; i < 32; ++i){
          esid += char(EEPROM.read(i));
        }
        Serial.println();
        Serial.print("SSID: ");
        Serial.println(esid);

        String epass = "";
        for (int i = 32; i < 96; ++i){
          epass += char(EEPROM.read(i));
        }
        Serial.print("PASS: ");
        Serial.println(epass);
}

void loop() {
  // put your main code here, to run repeatedly:
}