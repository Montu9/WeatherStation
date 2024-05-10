#ifndef BMP_H
#define BMP_H

#include <Adafruit_BMP280.h>

typedef struct {
  float temperature;
  float pressure;
} BMPData;

class BMP_sensor {
 public:
  BMP_sensor();
  BMPData readBMPData();
  float readBMPTemp();
  float readBMPPressure();

 private:
  Adafruit_BMP280 bmp;
};

#endif