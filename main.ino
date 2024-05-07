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
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/subscribe") {
      alertOn = HIGH;
      lastTimeBotAlerted = millis(); // Reset alert timer
      bot.sendMessage(chat_id, "You have subscribed to measurement updates", "");
    } else if (text == "/unsubscribe") {
      alertOn = LOW;
      bot.sendMessage(chat_id, "You have unsubscribed to measurement updates", "");
    } else if (text == "/led_on") {
      ledState = HIGH;
      digitalWrite(LED_PIN, ledState);
      bot.sendMessage(chat_id, "The LED is turned ON", "");
    } else if (text == "/led_off") {
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
      bot.sendMessage(chat_id, "The LED is turned OFF", "");
    } else if (text == "/state") {
      String message = digitalRead(LED_PIN) ? "LED is ON" : "LED is OFF";
      message += "\nYou are currently " + (alertOn ? "subscribed" : "unsubscribed");
      bot.sendMessage(chat_id, message, "");
    } else if (text == "/help") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "I'm Evergreen bot\n";
      welcome += "Use the following commands to control the LED.\n\n";
      welcome += "/subscribe to start receiving measurement updates\n";
      welcome += "/unsubscribe to stop receiving measurement updates\n";
      welcome += "/led_on to turn ON LED\n";
      welcome += "/led_off to turn OFF LED\n";
      welcome += "/state to request current LED state\n";
      welcome += "/help to see available commands\n";
      bot.sendMessage(chat_id, welcome, "");
    } else {
      bot.sendMessage(chat_id, "Unknown command. Please write /help to see available commands", "");
    }
  }
}

void readMessages() {
  int newMessageCount;
  do {
    newMessageCount = bot.getUpdates(bot.last_message_received + 1);
    Serial.println("Response Received!");
    handleMessages(newMessageCount);
  } while (newMessageCount);
}

void writeAlert() {
  String message = digitalRead(LED_PIN) ? "LED is ON" : "LED is OFF";
  message += "\nYou are receiving this message because you have subscribed to this topic.\n";
  message += "To unsubscribe write /unsubscribe\nTo see all available commands write /help";
  bot.sendMessage(chat_id, message, "");
}

void setupTelegram() {
  // Setup Telegram certificate
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  const String commands = F("["
    "{\"command\":\"subscribe\",   \"description\":\"Start receiving measurement updates\"},"
    "{\"command\":\"unsubscribe\", \"description\":\"Stop receiving measurement updates\"},"
    "{\"command\":\"led_on\",      \"description\":\"Turn ON LED\"},"
    "{\"command\":\"led_off\",     \"description\":\"Turn OFF LED\"},"
    "{\"command\":\"state\",       \"description\":\"Request current LED state\"},"
    "{\"command\":\"help\",        \"description\":\"See available commands\"}]");
  bot.setMyCommands(commands);
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  setupWifi();
  setupTelegram();
}

void loop() {
  if (millis() > lastTimeBotReadMessages + BOT_READ_MESSAGES_DELAY) {
    readMessages();
    lastTimeBotReadMessages = millis();
  }

  if (alertOn && millis() > lastTimeBotAlert + BOT_ALERT_DELAY) {
    writeAlert();
    lastTimeBotAlert = millis();
  }
}