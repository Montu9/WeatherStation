#ifndef SENSORSREADER_H
#define SENSORSREADER_H

#include <Arduino.h>
#include "aht.h"
#include "bmp.h"
#include "ldr.h"
#include "soil.h"
#include "SensorsData.h"

#include <array>
#include <cstddef>

class SensorsReader {
public:
  enum class Rating { VeryLow, Low, Ok, High, VeryHigh };

  SensorsReader(AHT_sensor* ahtSensor, BMP_sensor* bmpSensor,
                LDR_sensor* ldrSensor, SOIL_sensor* soilSensor);

  SensorsData readData() const;

  float readTemperature() const;
  float readHumidity() const;
  float readPressure() const;
  float readBrightness() const;
  float readSoilMoisture() const;

  Rating calculateSoilMoistureRating(float soilMoisture) const;

  String toString(SensorsData sensorsData) const;

  const char* ratingToString(Rating rating) const;

private:
  AHT_sensor* ahtSensor;
  BMP_sensor* bmpSensor;
  LDR_sensor* ldrSensor;
  SOIL_sensor* soilSensor;

  template <typename Value, typename Rating, size_t N>
  Rating calculate_rating(Value value,
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

#endif // SENSORSREADER_H
