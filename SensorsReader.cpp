#include "SensorsReader.h"

SensorsReader::SensorsReader(AHT_sensor* ahtSensor, BMP_sensor* bmpSensor,
                           LDR_sensor* ldrSensor, SOIL_sensor* soilSensor)
    : ahtSensor(ahtSensor), bmpSensor(bmpSensor),
      ldrSensor(ldrSensor), soilSensor(soilSensor) {}

SensorsData SensorsReader::readData() const {
  return {
      .ahtData = ahtSensor->readAHTData(),
      .bmpData = bmpSensor->readBMPData(),
      .ldrLight = ldrSensor->readLDRLight(),
      .soilMoist = soilSensor->readSOILMoist()
  };
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
  String output("AHT: temperature: ");
  output += String(sensorsData.ahtData.temperature, 2);
  output += " humidity: ";
  output += String(sensorsData.ahtData.humidity, 2);
  output += "\n";

  output += "BMP: temperature: ";
  output += String(sensorsData.bmpData.temperature, 2);
  output += " pressure: ";
  output += String(sensorsData.bmpData.pressure, 2);
  output += "\n";

  output += "Light value: ";
  output += String(sensorsData.ldrLight, 2);
  output += "%\n";

  output += "Soil moist value: ";
  output += String(sensorsData.soilMoist, 2);
  output += "%";

  return output;
}