#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "SensorsReader.h"
#include "ThingSpeak.h"
#include "aht.h"
#include "bmp.h"
#include "bot.h"
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

WiFiClientSecure client;
WiFiClient tsClient;
Bot* bot;

AHT_sensor* aht;
BMP_sensor* bmp;
LDR_sensor* ldr;
SOIL_sensor* soil;
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

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  // Setup sensors
  aht = new AHT_sensor();
  bmp = new BMP_sensor();
  ldr = new LDR_sensor(39);
  soil = new SOIL_sensor(36);
  sensorsReader = new SensorsReader(aht, bmp, ldr, soil);

  setupWifi();

  bot = new Bot(BOT_TOKEN, client, CHAT_ID, sensorsReader);
  bot->setLongPoll(BOT_READ_MESSAGES_POLLING_DURATION);

  storage = new Storage(tsClient);
}

void loop() {
  if (millis() > lastTimeBotReadMessages + BOT_READ_MESSAGES_DELAY) {
    bot->readMessages();
    lastTimeBotReadMessages = millis();
  }

  if (millis() > bot->lastTimeBotAlert + BOT_ALERT_DELAY) {
    const auto data = sensorsReader->readData();
    const auto moistureRating = sensorsReader->calculateSoilMoistureRating(data.soilMoisture);
    const bool soilMoistureIsNotOk = moistureRating != SensorsReader::Rating::Ok;

    if (bot->alertOn || soilMoistureIsNotOk) {
      bot->writeAlert(data);
    }
    bot->lastTimeBotAlert = millis();

    storage->setSensorsData(data);
    storage->saveData();
  }
}