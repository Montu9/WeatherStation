#include "bmp.h"

BMP_sensor::BMP_sensor() {
  while (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP sensor, check wiring!");
    delay(500);
  }

  Serial.println("BMP sensor correctly initialized!");
}

BMPData BMP_sensor::readBMPData() {
  BMPData BMPValues;

  float bmpTemp = bmp.readTemperature();

  Serial.println(F("Temperature...: "));
  Serial.print(bmpTemp);
  Serial.println(F(" +-0.1C"));

  float bmpPressure = bmp.readPressure() / 100.0 - 414;

  Serial.println(F("Pressure...: "));
  Serial.print(bmpPressure);
  Serial.println(F(" +-0.12hPa"));

  BMPValues.temperature = bmpTemp;
  BMPValues.pressure = bmpPressure;
  return BMPValues;
}

float BMP_sensor::readBMPTemp() {
  float bmpTemp = bmp.readTemperature();

  Serial.println(F("Temperature...: "));
  Serial.print(bmpTemp);
  Serial.println(F(" +-0.1C"));

  return bmpTemp;
}

float BMP_sensor::readBMPPressure() {
  float bmpPressure = bmp.readPressure() / 100.0 - 414;

  Serial.println(F("Pressure...: "));
  Serial.print(bmpPressure);
  Serial.println(F(" +-0.12hPa"));

  return bmpPressure;
}