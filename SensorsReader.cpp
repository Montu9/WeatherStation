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
  return {.temperature = bmpSensor->readBMPTemp(),
          .humidity = ahtSensor->readAHTHumidity(),
          .pressure = bmpSensor->readBMPPressure(),
          .brightness = ldrSensor->readLDRLight(),
          .soilMoisture = soilSensor->readSOILMoist()};
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

float SensorsReader::readSoilMoisture() const {
  return soilSensor->readSOILMoist();
}

SensorsReader::Rating SensorsReader::calculateSoilMoistureRating(float soilMoisture) const {
  const std::array<Rating, 5> ratings{Rating::VeryLow, Rating::Low, Rating::Ok, Rating::High,
                                      Rating::VeryHigh};

  const std::array<float, 4> soilMoistureThresholds{
    SOIL_MOISTURE_VERY_LOW_THRESHOLD, SOIL_MOISTURE_LOW_THRESHOLD, SOIL_MOISTURE_OK_THRESHOLD,
    SOIL_MOISTURE_HIGH_THRESHOLD};

  return calculate_rating<float, Rating>(soilMoisture, soilMoistureThresholds, ratings);
}

String SensorsReader::toString(SensorsData sensorsData) const {
  String output("");

  const auto soilMoistureRating = calculateSoilMoistureRating(sensorsData.soilMoisture);
  const auto soilMoistureRatingStr = ratingToString(soilMoistureRating);
  if (soilMoistureRating != Rating::Ok) {
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
  output += String(sensorsData.soilMoisture, 2);
  output += "% (";
  output += soilMoistureRatingStr;
  output += ")";

  return output;
}

const char* SensorsReader::ratingToString(Rating rating) const {
  switch (rating) {
    case Rating::VeryLow:
      return "Very Low";
    case Rating::Low:
      return "Low";
    case Rating::Ok:
      return "Ok";
    case Rating::High:
      return "High";
    case Rating::VeryHigh:
    default:
      return "Very High";
  }
}
