#include "aht.h"

AHT_sensor::AHT_sensor() {
  while (!aht21.begin()) {
    Serial.println("Could not find a valid AHT21 sensor, check wiring!");
    delay(500);
  }

  Serial.println("AHT21 sensor correctly initialized!");
}

AHTData AHT_sensor::readAHTData() {
  AHTData AHTValues;

  float ahtTemp = aht21.readTemperature();

  Serial.print(F("Temperature...: "));
  if (ahtTemp != AHTXX_ERROR) {
    Serial.print(ahtTemp);
    Serial.println(F(" +-0.3C"));
  } else {
    printAHTStatus();
  }

  float ahtHumidity = aht21.readHumidity(AHTXX_USE_READ_DATA);

  Serial.print(F("Humidity...: "));
  if (ahtHumidity != AHTXX_ERROR) {
    Serial.print(ahtHumidity);
    Serial.println(F(" +-2%"));
  } else {
    printAHTStatus();
  }
  AHTValues.temperature = ahtTemp;
  AHTValues.humidity = ahtHumidity;
  return AHTValues;
}

float AHT_sensor::readAHTTemp() {
  float ahtTemp = aht21.readTemperature();

  Serial.print(F("Temperature...: "));
  if (ahtTemp != AHTXX_ERROR) {
    Serial.print(ahtTemp);
    Serial.println(F(" +-0.3C"));
  } else {
    printAHTStatus();
  }

  return ahtTemp;
}

float AHT_sensor::readAHTHumidity() {
  float ahtHumidity = aht21.readHumidity();

  Serial.print(F("Humidity...: "));
  if (ahtHumidity != AHTXX_ERROR) {
    Serial.print(ahtHumidity);
    Serial.println(F(" +-2%"));
  } else {
    printAHTStatus();
  }

  return ahtHumidity;
}

void AHT_sensor::printAHTStatus() {
  switch (aht21.getStatus()) {
    case AHTXX_NO_ERROR:
      Serial.println(F("no error"));
      break;

    case AHTXX_BUSY_ERROR:
      Serial.println(F("sensor busy, increase polling time"));
      break;

    case AHTXX_ACK_ERROR:
      Serial.println(
        F("sensor didn't return ACK, not connected, broken, long wires (reduce speed), bus locked "
          "by slave (increase stretch limit)"));
      break;

    case AHTXX_DATA_ERROR:
      Serial.println(
        F("received data smaller than expected, not connected, broken, long wires (reduce speed), "
          "bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_CRC8_ERROR:
      Serial.println(
        F("computed CRC8 not match received CRC8, this feature supported only by AHT2x sensors"));
      break;

    default:
      Serial.println(F("unknown status"));
      break;
  }
}