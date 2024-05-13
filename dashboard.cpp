#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "LittleFS.h"
#include "dashboard.h"

String Dashboard::processor(const String& var) {
  SensorsData values = storage->getSensorData();
  if (var == "MOISTURE_VAL") {
    String output("(");
    output += ratingToString(values.soilMoisture.rating);
    output += ") ";
    output += values.soilMoisture.value;
    return output;
  } else if (var == "BRIGHTNESS_VAL") {
    return String(values.brightness);
  } else if (var == "HUMIDITY_VAL") {
    return String(values.humidity);
  } else if (var == "TEMPERATURE_VAL") {
    return String(values.temperature);
  } else if (var == "PRESSURE_VAL") {
    return String((int)values.pressure);
  }

  return String();
}

Storage* Dashboard::getStorage() {
  return storage;
}

Dashboard::Dashboard(AsyncWebServer& server, Storage* storageTemp)
    : webServer(server), storage(storageTemp) {
    if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  webServer.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/index.html", String(), false,
                  [this](const String& var) { return this->processor(var); });
  });

  webServer.on("/update", HTTP_GET, [this](AsyncWebServerRequest* request) {
    SensorsData values = storage->getSensorData();
    StaticJsonDocument<100> data;

    String output("(");
    output += ratingToString(values.soilMoisture.rating);
    output += ") ";
    output += values.soilMoisture.value;
    data["brightness"] = String(values.brightness);
    data["temperature"] = String(values.temperature);
    data["soilMoisture"] = output;
    data["humidity"] = String(values.humidity);
    data["pressure"] = String((int)values.pressure);

    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
  });

  // Start server
  webServer.begin();
}
