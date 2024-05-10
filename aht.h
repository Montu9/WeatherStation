#ifndef AHT_H
#define AHT_H

#include <AHTxx.h>
#include <Wire.h>

typedef struct {
  float temperature;
  float humidity;
} AHTData;

class AHT_sensor {
 public:
  AHT_sensor();
  AHTData readAHTData();
  float readAHTTemp();
  float readAHTHumidity();
  void printAHTStatus();

 private:
  AHTxx aht21{AHTxx(AHTXX_ADDRESS_X38, AHT2x_SENSOR)};
};

#endif