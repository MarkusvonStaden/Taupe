#include <Arduino.h>
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  EEPROM.begin(96);
  for(unsigned int i = 0; i < EEPROM.length(); i++){
    EEPROM.write(i, 0);
  }
EEPROM.commit();

  Serial.println(EEPROM.length());
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}