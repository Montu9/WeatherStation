#include "SensorsReader.h"

SensorsReader::SensorsReader(AHT_sensor* ahtSensor, BMP_sensor* bmpSensor,
                           LDR_sensor* ldrSensor, SOIL_sensor* soilSensor)
    : ahtSensor(ahtSensor), bmpSensor(bmpSensor),
      ldrSensor(ldrSensor), soilSensor(soilSensor) {}

SensorsData SensorsReader::readData() const {
  return SensorsData(bmpSensor->readBMPTemp(), ahtSensor->readAHTHumidity(),
                     bmpSensor->readBMPPressure(), ldrSensor->readLDRLight(),
                     soilSensor->readSOILMoist());
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

String SensorsReader::toString(SensorsData sensorsData) const {
  String output("");

  const auto soilMoistureRatingStr = rankingToString(sensorsData.soilMoistureRating);
  if (sensorsData.soilMoistureRating != SensorsData::Rating::Ok) {
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

const char* SensorsReader::rankingToString(SensorsData::Rating rating) const {
  switch (rating) {
  case SensorsData::Rating::VeryLow:
    return "Very Low";
  case SensorsData::Rating::Low:
    return "Low";
  case SensorsData::Rating::Ok:
    return "Ok";
  case SensorsData::Rating::High:
    return "High";
  case SensorsData::Rating::VeryHigh:
  default:
    return "Very High";
  }
}
