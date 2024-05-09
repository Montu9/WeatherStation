#ifndef SENSORSREADER_H
#define SENSORSREADER_H

#include <Arduino.h>
#include "aht.h"
#include "bmp.h"
#include "ldr.h"
#include "soil.h"
#include "SensorsData.h"

class SensorsReader {
public:
  SensorsReader(AHT_sensor* ahtSensor, BMP_sensor* bmpSensor,
                LDR_sensor* ldrSensor, SOIL_sensor* soilSensor);

  SensorsData readData() const;

  float readTemperature() const;
  float readHumidity() const;
  float readPressure() const;
  float readBrightness() const;
  float readSoilMoisture() const;

  String toString(SensorsData sensorsData) const;

private:
  AHT_sensor* ahtSensor;
  BMP_sensor* bmpSensor;
  LDR_sensor* ldrSensor;
  SOIL_sensor* soilSensor;

  const char* rankingToString(SensorsData::Rating rating) const;
};

#endif // SENSORSREADER_H
