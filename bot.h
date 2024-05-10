#ifndef BOT_H
#define BOT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "SensorsData.h"
#include "SensorsReader.h"

class Bot {
 public:
  Bot(const String& token, WiFiClientSecure& client, String chatId, SensorsReader* sensorsReader);

  void readMessages();

  void writeAlert(SensorsData sensorsData);

  void setLongPoll(int longPoll);

  unsigned long lastTimeBotAlert = 0;

  bool alertOn = false;

 private:
  UniversalTelegramBot bot;
  String chatId;
  SensorsReader* sensorsReader;

  void handleMessages(int messageCount);
};

#endif  // BOT_H
