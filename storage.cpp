#include "storage.h"
#include "secrets.h"

Storage::Storage(WiFiClient& client) {
  ThingSpeak.begin(client);
  sensorsData = {.temperature = 0.0,
                 .humidity = 0.0,
                 .pressure = 0.0,
                 .brightness = 0.0,
                 .soilMoisture = {.value = 0.0, .rating = Rating::VeryLow}};
}

void Storage::setSensorsData(SensorsData data) {
  sensorsData = data;
  ThingSpeak.setField(1, sensorsData.temperature);
  ThingSpeak.setField(2, sensorsData.humidity);
  ThingSpeak.setField(3, sensorsData.pressure);
  ThingSpeak.setField(4, sensorsData.brightness);
  ThingSpeak.setField(5, sensorsData.soilMoisture.value);
}

void Storage::saveData() {
  int x = ThingSpeak.writeFields(TS_CHANNEL_NUM, TS_WRITE_API_KEY);

  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

SensorsData Storage::getSensorData() {
  return sensorsData;
}