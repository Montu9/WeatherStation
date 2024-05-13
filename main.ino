#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "SensorsReader.h"
#include "ThingSpeak.h"
#include "aht.h"
#include "bmp.h"
#include "bot.h"
#include "dashboard.h"
#include "ldr.h"
#include "secrets.h"
#include "soil.h"
#include "storage.h"

// secrets header must define these:
// WI-FI
// #define WIFI_SSID "ADD_YOUR_SSID"
// #define WIFI_PASS "ADD_YOUR_PASS"
// Telegram
// #define BOT_TOKEN "ADD_YOUR_TOKEN"
// #define CHAT_ID "ADD_YOUR_CHAT_ID"
// ThingSpeak
// #define TS_CHANNEL_NUM (Add your channel e.g. 123UL)
// #define TS_WRITE_API_KEY "ADD_YOUR_API_KEY"

AsyncWebServer server(80);
Dashboard* dashboard;
WiFiClientSecure client;
WiFiClient tsClient;
Bot* bot;

AHT_sensor* aht;    // GPIO-22 (I2C_SCL) oraz GPIO-21 (I2C_SDA)
BMP_sensor* bmp;    // GPIO-22 (I2C_SCL) oraz GPIO-21 (I2C_SDA)
LDR_sensor* ldr;    // GPIO-36 (ADC1_0)
SOIL_sensor* soil;  // GPIO-39 (ADC1_3)
SensorsReader* sensorsReader;
Storage* storage;

// 10 seconds
#define BOT_READ_MESSAGES_POLLING_DURATION 10
// 1 second
#define BOT_READ_MESSAGES_DELAY 1000
unsigned long lastTimeBotReadMessages = 0;

// 1 minute
#define BOT_ALERT_DELAY (1000 * 60 * 1)

void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());
}

void handleReadMessages() {
  bot->readMessages();
}

void handleBotAlert() {
  const auto data = sensorsReader->readData();

  if (bot->alertOn() || sensorsReader->isSoilMoistureLevelCritical(data.soilMoisture)) {
    bot->writeAlert(data);
  }

  storage->setSensorsData(data);
  storage->saveData();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  // Setup sensors
  aht = new AHT_sensor();
  bmp = new BMP_sensor();
  ldr = new LDR_sensor(36);
  soil = new SOIL_sensor(39);
  sensorsReader = new SensorsReader(aht, bmp, ldr, soil);

  setupWifi();

  bot = new Bot(BOT_TOKEN, client, CHAT_ID, sensorsReader);
  bot->setLongPoll(BOT_READ_MESSAGES_POLLING_DURATION);

  storage = new Storage(tsClient);
  dashboard = new Dashboard(server, storage);

  handleBotAlert();
}

void loop() {
  if (millis() > lastTimeBotReadMessages + BOT_READ_MESSAGES_DELAY) {
    handleReadMessages();
    lastTimeBotReadMessages = millis();
  }

  if (millis() > bot->lastTimeBotAlert + BOT_ALERT_DELAY) {
    handleBotAlert();
    bot->lastTimeBotAlert = millis();
  }
}