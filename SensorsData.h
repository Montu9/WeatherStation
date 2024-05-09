#ifndef SENSORSDATA_H
#define SENSORSDATA_H

#include <array>
#include <cstddef>

class SensorsData {
public:
  enum class Rating { VeryLow, Low, Ok, High, VeryHigh };

  float temperature;
  float humidity;
  float pressure;
  float brightness;
  float soilMoisture;
  Rating soilMoistureRating;

  SensorsData(float temperature, float humidity, float pressure,
              float brightness, float soilMoisture);

private:
  template <typename Value, typename Rating, size_t N>
  Rating calculate_rating(Value value,
                          const std::array<Value, N - 1>& thresholds,
                          const std::array<Rating, N>& ratings) {
    for (size_t i = 0; i < N - 1; ++i) {
      if (value < thresholds[i]) {
        return ratings[i];
      }
    }

    return ratings[N - 1];
  }
};

#endif // SENSORSDATA_H
