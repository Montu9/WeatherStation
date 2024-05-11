#ifndef SENSORSREADER_H
#define SENSORSREADER_H

#include <Arduino.h>
#include "SensorsData.h"
#include "aht.h"
#include "bmp.h"
#include "ldr.h"
#include "soil.h"

#include <array>
#include <cstddef>

class SensorsReader {
 public:
  SensorsReader(AHT_sensor* ahtSensor,
                BMP_sensor* bmpSensor,
                LDR_sensor* ldrSensor,
                SOIL_sensor* soilSensor);

  SensorsData readData() const;

  float readTemperature() const;
  float readHumidity() const;
  float readPressure() const;
  float readBrightness() const;
  SoilMoisture readSoilMoisture() const;

  String toString(SensorsData sensorsData) const;

  bool isSoilMoistureLevelCritical(SoilMoisture soilMoisture) const;

 private:
  AHT_sensor* ahtSensor;
  BMP_sensor* bmpSensor;
  LDR_sensor* ldrSensor;
  SOIL_sensor* soilSensor;

  Rating calculateSoilMoistureRating(float soilMoisture) const;

  template <typename Value, typename Rating, size_t N>
  Rating calculateRating(Value value,
                         const std::array<Value, N - 1>& thresholds,
                         const std::array<Rating, N>& ratings) const {
    for (size_t i = 0; i < N - 1; ++i) {
      if (value < thresholds[i]) {
        return ratings[i];
      }
    }

    return ratings[N - 1];
  }
};

#endif  // SENSORSREADER_H
