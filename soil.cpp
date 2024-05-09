#include <Arduino.h>
#include "soil.h"

SOIL_sensor::SOIL_sensor(int pin_num) {
  soil_pin = pin_num;
  Serial.println("SOIL sensor correctly initialized!");
}

float SOIL_sensor::readSOILMoist() {
  int value = analogRead(soil_pin);
  float moistPerc = (float)value / 4905.f;
  moistPerc *= 100;
  // escape % -> %%
  Serial.printf("Moist value: %d = %.2f%%\n", value, moistPerc);
  return moistPerc;
}