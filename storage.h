#ifndef STORAGE_H
#define STORAGE_H

#include <WiFi.h>
#include "SensorsData.h"
#include "ThingSpeak.h"

class Storage {
 public:
  Storage(WiFiClient& client);
  void setSensorsData(SensorsData data);
  void saveData();
  SensorsData getSensorData();

 private:
  SensorsData sensorsData;
};

#endif  // STORAGE_H
