#include <Arduino.h>
#include "ldr.h"

LDR_sensor::LDR_sensor(int pin_num) {
  ldr_pin = pin_num;
  Serial.println("LDR sensor correctly initialized!");
}

float LDR_sensor::readLDRLight() {
  int value = analogRead(ldr_pin);
  float lightPerc = 1 - ((float)value / 4905.f);
  lightPerc *= 100;
  // escape % -> %%
  Serial.printf("Light value: %d = %.2f%%\n", value, lightPerc);
  return lightPerc;
}