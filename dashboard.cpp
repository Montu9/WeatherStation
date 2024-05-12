#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include "dashboard.h"

String Dashboard::processor(const String& var) {
  SensorsData values = storage->getSensorData();
  if (var == "MOISTURE_VAL") {
    String output(values.soilMoisture.value);
    output += "(";
    output += ratingToString(values.soilMoisture.rating);
    output += ")";
    return output;
  } else if (var == "BRIGHTNESS_VAL") {
    return String(values.brightness);
  } else if (var == "HUMIDITY_VAL") {
    return String(values.humidity);
  } else if (var == "TEMPERATURE_VAL") {
    return String(values.temperature);
  } else if (var == "PRESSURE_VAL") {
    return String(values.pressure);
  }

  return String();
}

Dashboard::Dashboard(AsyncWebServer& server, Storage* storage)
    : webServer(server), storage(storage) {
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  webServer.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
    request->send(LittleFS, "/index.html", String(), false,
                  [this](const String& var) { return this->processor(var); });
  });

  // Start server
  webServer.begin();
}
