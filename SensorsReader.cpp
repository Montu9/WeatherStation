#include "SensorsReader.h"

constexpr static float SOIL_MOISTURE_VERY_LOW_THRESHOLD = 10.f;
constexpr static float SOIL_MOISTURE_LOW_THRESHOLD = 30.f;
constexpr static float SOIL_MOISTURE_OK_THRESHOLD = 60.f;
constexpr static float SOIL_MOISTURE_HIGH_THRESHOLD = 80.f;

SensorsReader::SensorsReader(AHT_sensor* ahtSensor,
                             BMP_sensor* bmpSensor,
                             LDR_sensor* ldrSensor,
                             SOIL_sensor* soilSensor)
    : ahtSensor(ahtSensor), bmpSensor(bmpSensor), ldrSensor(ldrSensor), soilSensor(soilSensor) {}

SensorsData SensorsReader::readData() const {
  return {.temperature = readTemperature(),
          .humidity = readHumidity(),
          .pressure = readPressure(),
          .brightness = readBrightness(),
          .soilMoisture = readSoilMoisture()};
}

float SensorsReader::readTemperature() const {
  return bmpSensor->readBMPTemp();
}

float SensorsReader::readHumidity() const {
  return ahtSensor->readAHTHumidity();
}

float SensorsReader::readPressure() const {
  return bmpSensor->readBMPPressure();
}

float SensorsReader::readBrightness() const {
  return ldrSensor->readLDRLight();
}

SoilMoisture SensorsReader::readSoilMoisture() const {
  const auto value = soilSensor->readSOILMoist();
  const auto rating = calculateSoilMoistureRating(value);
  return {.value = value, .rating = rating};
}

String SensorsReader::toString(SensorsData sensorsData) const {
  String output("");

  const auto soilMoistureRatingStr = ratingToString(sensorsData.soilMoisture.rating);
  if (isSoilMoistureLevelCritical(sensorsData.soilMoisture)) {
    output += "Warning, current soil moisture is ";
    output += soilMoistureRatingStr;
    output += "\n";
  }

  output += String("Temperature: ");
  output += String(sensorsData.temperature, 2);
  output += "℃\nPressure: ";
  output += String(sensorsData.pressure, 2);
  output += "hPa\n";

  output += "Humidity: ";
  output += String(sensorsData.humidity, 2);
  output += "%\n";

  output += "Brightness: ";
  output += String(sensorsData.brightness, 2);
  output += "%\n";

  output += "Soil moisture: ";
  output += String(sensorsData.soilMoisture.value, 2);
  output += "% (";
  output += soilMoistureRatingStr;
  output += ")";

  return output;
}

bool SensorsReader::isSoilMoistureLevelCritical(SoilMoisture soilMoisture) const {
  return soilMoisture.rating == Rating::VeryLow;
}

Rating SensorsReader::calculateSoilMoistureRating(float soilMoisture) const {
  const std::array<Rating, 5> ratings{Rating::VeryLow, Rating::Low, Rating::Ok, Rating::High,
                                      Rating::VeryHigh};

  const std::array<float, 4> thresholds{SOIL_MOISTURE_VERY_LOW_THRESHOLD,
                                        SOIL_MOISTURE_LOW_THRESHOLD, SOIL_MOISTURE_OK_THRESHOLD,
                                        SOIL_MOISTURE_HIGH_THRESHOLD};

  return calculateRating<float, Rating>(soilMoisture, thresholds, ratings);
}
