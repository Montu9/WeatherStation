#include <Arduino.h>
#include "ldr.h"

LDR_sensor::LDR_sensor(int pin_num) {
  ldr_pin = pin_num;
  Serial.println("LDR sensor correctly initialized!");
}

int LDR_sensor::readLDRLight() {
  int value = analogRead(ldr_pin);
  Serial.print("Light value: ");
  Serial.println(value);
  return value;
}