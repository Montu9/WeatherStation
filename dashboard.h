#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <ESPAsyncWebServer.h>
#include "storage.h"

class Dashboard {
 private:
  AsyncWebServer& webServer;
  Storage* storage;

 public:
  Dashboard(AsyncWebServer& server, Storage* storageTemp);
  String processor(const String& var);
  Storage* getStorage();
};

#endif  // DASHBOARD_H
