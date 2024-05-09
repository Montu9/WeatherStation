#include <Arduino.h>
#include "soil.h"

SOIL_sensor::SOIL_sensor(int pin_num) {
  soil_pin = pin_num;
  Serial.println("SOIL sensor correctly initialized!");
}

int SOIL_sensor::readSOILMoist() {
  int value = analogRead(soil_pin);
  Serial.print("Light value: ");
  Serial.println(value);
  return value;
}