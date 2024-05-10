#include "storage.h"
#include "secrets.h"

Storage::Storage(WiFiClient& client) {
  ThingSpeak.begin(client); 
}

void Storage::setSensorsData(SensorsData data) {
  sensorsData = data;
  ThingSpeak.setField(1, sensorsData.temperature);
  ThingSpeak.setField(2, sensorsData.humidity);
  ThingSpeak.setField(3, sensorsData.pressure);
  ThingSpeak.setField(4, sensorsData.brightness);
  ThingSpeak.setField(5, sensorsData.soilMoisture);
}

void Storage::saveData() {
  int x = ThingSpeak.writeFields(TS_CHANNEL_NUM, TS_WRITE_API_KEY);

  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}