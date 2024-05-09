#include "SensorsData.h"

constexpr static float SOIL_MOISTURE_VERY_LOW_THRESHOLD = 10.f;
constexpr static float SOIL_MOISTURE_LOW_THRESHOLD = 30.f;
constexpr static float SOIL_MOISTURE_OK_THRESHOLD = 60.f;
constexpr static float SOIL_MOISTURE_HIGH_THRESHOLD = 80.f;

SensorsData::SensorsData(float temperature, float humidity, float pressure,
                         float brightness, float soilMoisture)
    : temperature(temperature), humidity(humidity), pressure(pressure),
      brightness(brightness), soilMoisture(soilMoisture) {
  const std::array<Rating, 5> ratings{Rating::VeryLow, Rating::Low, Rating::Ok,
                                      Rating::High, Rating::VeryHigh};

  const std::array<float, 4> soilMoistureThresholds{
      SOIL_MOISTURE_VERY_LOW_THRESHOLD,
      SOIL_MOISTURE_LOW_THRESHOLD,
      SOIL_MOISTURE_OK_THRESHOLD,
      SOIL_MOISTURE_HIGH_THRESHOLD
  };
  soilMoistureRating = calculate_rating<float, Rating>(
      soilMoisture, soilMoistureThresholds, ratings);
}