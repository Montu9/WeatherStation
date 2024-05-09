#ifndef SENSORSREADER_H
#define SENSORSREADER_H

#include <Arduino.h>
#include "aht.h"
#include "bmp.h"
#include "ldr.h"
#include "soil.h"

struct SensorsData {
  AHTData ahtData;
  BMPData bmpData;
  int ldrLight;
  int soilMoist;
};

class SensorsReader {
public:
  SensorsReader(AHT_sensor* ahtSensor, BMP_sensor* bmpSensor,
               LDR_sensor* ldrSensor, SOIL_sensor* soilSensor);

  SensorsData readData() const;

  String toString(SensorsData sensorsData) const;

private:
  AHT_sensor* ahtSensor;
  BMP_sensor* bmpSensor;
  LDR_sensor* ldrSensor;
  SOIL_sensor* soilSensor;
};

#endif // SENSORSREADER_H