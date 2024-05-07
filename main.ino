// https://www.electronicwings.com/esp32/control-the-led-using-telegram-message-and-esp32
// https://www.electronicwings.com/esp32/temperature-alert-on-telegram-using-esp32

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "ADD_YOUR_SSID";
const char* password = "ADD_YOUR_PASS";

#define BOT_TOKEN "ADD_YOUR_TOKEN"
#define CHAT_ID "ADD_YOUR_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// 10 seconds
#define BOT_READ_MESSAGES_POLLING_DURATION 10
// 1 second
#define BOT_READ_MESSAGES_DELAY 1000
unsigned long lastTimeBotReadMessages = 0;

// 1 minute
#define BOT_ALERT_DELAY (1000 * 60 * 1)
unsigned long lastTimeBotAlert = 0;
bool alertOn = LOW;

#define LED_PIN 2
bool ledState = LOW;

void handleMessages(int messageCount) {
  Serial.println("handleMessages");
  Serial.println(String(messageCount));

  for (int i = 0; i < messageCount; ++i) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      Serial.printf("Unauthorized user: %s\n", chat_id.c_str());
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String from = bot.messages[i].from_name;
    String text = bot.messages[i].text;
    Serial.printf("Command: %s from: %s\n", text.c_str(), from.c_str());

    String output = "";

    if (text == "/start") {
      output = "Welcome, " + from + ".\n";
      output += "I'm Evergreen bot\n";
      output += "What would you like to know?\n";
      output += "Type /help to see available commands\n";
    } else if (text == "/subscribe") {
      alertOn = HIGH;
      lastTimeBotAlerted = millis(); // Reset alert timer
      output = "You have subscribed to measurement updates";
    } else if (text == "/unsubscribe") {
      alertOn = LOW;
      output = "You have unsubscribed to measurement updates";
    } else if (text == "/led_on") {
      ledState = HIGH;
      digitalWrite(LED_PIN, ledState);
      output = "The LED is turned ON";
    } else if (text == "/led_off") {
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
      output = "The LED is turned OFF";
    } else if (text == "/state") {
      output = digitalRead(LED_PIN) ? "LED is ON" : "LED is OFF";
      output += "\nYou are currently " + (alertOn ? "subscribed" : "unsubscribed");
    } else if (text == "/help") {
      output = "Welcome, " + from + ".\n";
      output += "I'm Evergreen bot\n";
      output += "Use the following commands to control the LED.\n\n";
      output += "/subscribe to start receiving measurement updates\n";
      output += "/unsubscribe to stop receiving measurement updates\n";
      output += "/led_on to turn ON LED\n";
      output += "/led_off to turn OFF LED\n";
      output += "/state to request current LED state\n";
      output += "/help to see available commands";
    } else {
      output = "Unknown command. Please type /help to see available commands";
    }

    Serial.printf("Sending to user: %s\n", output.c_str());
    bot.sendMessage(chat_id, output, "");
  }
}

void writeAlert() {
  String message = digitalRead(LED_PIN) ? "LED is ON" : "LED is OFF";
  message += "\nYou are receiving this message because you have subscribed to this topic.\n";
  message += "To unsubscribe type /unsubscribe\nTo see all available commands type /help";
  Serial.printf("Writing alert: %s\n", message.c_str());
  bot.sendMessage(chat_id, message, "");
}

void readMessagesPolling() {
  // Long poll on first read
  bot.longPoll = BOT_READ_MESSAGES_POLLING_DURATION;
  int newMessageCount = bot.getUpdates(bot.last_message_received + 1);

  // Don't long poll consequent reads
  bot.longPoll = 0;

  while (newMessageCount) {
    Serial.println("Received message");
    handleMessages(newMessageCount);
    int newMessageCount = bot.getUpdates(bot.last_message_received + 1);
  }
}

void setupTelegram() {
  // Setup Telegram certificate
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  bot.setMyCommands(F("["
    "{\"command\":\"subscribe\",   \"description\":\"Start receiving measurement updates\"},"
    "{\"command\":\"unsubscribe\", \"description\":\"Stop receiving measurement updates\"},"
    "{\"command\":\"led_on\",      \"description\":\"Turn ON LED\"},"
    "{\"command\":\"led_off\",     \"description\":\"Turn OFF LED\"},"
    "{\"command\":\"state\",       \"description\":\"Request current LED state\"},"
    "{\"command\":\"help\",        \"description\":\"See available commands\"}]"));
}

void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  setupWifi();
  setupTelegram();
}

void loop() {
  if (millis() > lastTimeBotReadMessages + BOT_READ_MESSAGES_DELAY) {
    readMessagesPolling();
    lastTimeBotReadMessages = millis();
  }

  if (alertOn && millis() > lastTimeBotAlert + BOT_ALERT_DELAY) {
    writeAlert();
    lastTimeBotAlert = millis();
  }
}