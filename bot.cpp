#include "bot.h"

constexpr static const char* SHOW_MEASUREMENTS_REPLY = R"--(Show measurements \uD83D\uDCDD)--";
constexpr static const char* SHOW_SOIL_MOISTURE_REPLY = R"--(Show soil moisture \uD83D\uDCA7)--";
constexpr static const char* SUBSCRIBE_REPLY = R"--(Subscribe \uD83D\uDD14)--";
constexpr static const char* UNSUBSCRIBE_REPLY = R"--(Unsubscribe \uD83D\uDD15)--";

Bot::Bot(const String& token, WiFiClientSecure& client, String chatId, SensorsReader* sensorsReader)
    : bot(token, client), chatId(chatId), sensorsReader(sensorsReader) {
  // Setup Telegram certificate
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  bot.setMyCommands(
    F("["
      "{\"command\":\"start\",       \"description\":\"Learn more about bot\"},"
      "{\"command\":\"subscribe\",   \"description\":\"Start receiving measurement updates\"},"
      "{\"command\":\"unsubscribe\", \"description\":\"Stop receiving measurement updates\"},"
      "{\"command\":\"state\",       \"description\":\"Request current sensor measurements\"},"
      "{\"command\":\"temperature\", \"description\":\"Request current temperature\"},"
      "{\"command\":\"humidity\",    \"description\":\"Request current humidity\"},"
      "{\"command\":\"pressure\",    \"description\":\"Request current pressure\"},"
      "{\"command\":\"brightness\",  \"description\":\"Request current brightness\"},"
      "{\"command\":\"moisture\",    \"description\":\"Request current soil moisture\"},"
      "{\"command\":\"help\",        \"description\":\"See available commands\"}]"));
}

void Bot::readMessages() {
  int newMessageCount = bot.getUpdates(bot.last_message_received + 1);

  // Don't long poll consequent reads
  const int originalLongPoll = bot.longPoll;
  bot.longPoll = 0;

  while (newMessageCount) {
    Serial.println("Bot :: Received message");
    handleMessages(newMessageCount);
    newMessageCount = bot.getUpdates(bot.last_message_received + 1);
  }

  // Revert to original long poll
  bot.longPoll = originalLongPoll;
}

void Bot::writeAlert(SensorsData sensorsData) {
  String message = sensorsReader->toString(sensorsData);
  message += "\n";

  if (alertOn) {
    message += "You are receiving this message, because you have subscribed to this topic.\n";
    message += "To unsubscribe type /unsubscribe\nTo see all available commands type /help";
  } else if (sensorsReader->isSoilMoistureLevelCritical(sensorsData.soilMoisture)) {
    message += "You are receiving this message, because soil moisture is at critical level.\n";
  }

  Serial.printf("Bot :: Writing alert: %s\n", message.c_str());
  bot.sendMessage(chatId, message, "");
}

void Bot::setLongPoll(int longPoll) {
  bot.longPoll = longPoll;
}

void Bot::handleMessages(int messageCount) {
  Serial.printf("Bot :: handleMessages %d\n", messageCount);

  for (int i = 0; i < messageCount; ++i) {
    String currentChatId = String(bot.messages[i].chat_id);
    if (currentChatId != chatId) {
      Serial.printf("Bot :: Unauthorized user: %s\n", currentChatId.c_str());
      bot.sendMessage(currentChatId, "Unauthorized user", "");
      continue;
    }

    String from = bot.messages[i].from_name;
    String text = bot.messages[i].text;
    Serial.printf("Bot :: Command: %s from: %s\n", text.c_str(), from.c_str());

    const auto mapReplyKeyboardToCommand = [&] {
      if (text == SHOW_MEASUREMENTS_REPLY) {
        text = "/state";
      } else if (text == SHOW_SOIL_MOISTURE_REPLY) {
        text = "/moisture";
      } else if (text == SUBSCRIBE_REPLY) {
        text = "/subscribe";
      } else if (text == UNSUBSCRIBE_REPLY) {
        text = "/unsubscribe";
      }
    };
    mapReplyKeyboardToCommand();

    String output = "";

    if (text == "/start") {
      output = "Welcome, " + from + ".\n";
      output += "I'm Evergreen bot\n";
      output += "What would you like to know?\n";
      output += "Type /help to see available commands\n";
    } else if (text == "/subscribe") {
      output = alertOn ? "You are already subscribing to measurement updates"
                       : "You have subscribed to measurement updates";
      // State changed
      if (!alertOn) {
        lastTimeBotAlert = millis();  // Reset alert timer
        alertOn = true;
        setReplyKeyboard();
      }
    } else if (text == "/unsubscribe") {
      // State changed
      if (alertOn) {
        alertOn = false;
        setReplyKeyboard();
      }
      output = "You have unsubscribed to measurement updates";
    } else if (text == "/state") {
      output = sensorsReader->toString(sensorsReader->readData());
      output += "\nYou are currently ";
      output += alertOn ? "subscribed" : "unsubscribed";
    } else if (text == "/temperature") {
      float temp = sensorsReader->readTemperature();
      output = String(temp, 2);
    } else if (text == "/humidity") {
      float humidity = sensorsReader->readHumidity();
      output = String(humidity, 2);
    } else if (text == "/pressure") {
      float pressure = sensorsReader->readPressure();
      output = String(pressure, 2);
    } else if (text == "/brightness") {
      float brightness = sensorsReader->readBrightness();
      output = String(brightness, 2);
      output += "%";
    } else if (text == "/moisture") {
      const auto moisture = sensorsReader->readSoilMoisture();
      output = String(moisture.value, 2);
      output += "% (";
      output += ratingToString(moisture.rating);
      output += ")";
    } else if (text == "/help") {
      output = "Welcome, " + from + ".\n";
      output += "I'm Evergreen bot\n";
      output += "Use the following commands to control the LED.\n\n";
      output += "/start to learn more about bot\n";
      output += "/subscribe to start receiving measurement updates\n";
      output += "/unsubscribe to stop receiving measurement updates\n";
      output += "/state to request current sensor measurements\n";
      output += "/temperature to request current temperature\n";
      output += "/humidity to request current humidity\n";
      output += "/pressure to request current pressure\n";
      output += "/brightness to request current brightness\n";
      output += "/moisture to request current soil moisture\n";
      output += "/help to see available commands";
    } else {
      output = "Unknown command. Please type /help to see available commands";
    }

    Serial.printf("Bot :: Sending to user: %s\n", output.c_str());
    bot.sendMessage(currentChatId, output, "");
  }
}

void Bot::setReplyKeyboard() {
  String keyboardJson("[[\"");
  keyboardJson += SHOW_MEASUREMENTS_REPLY;
  keyboardJson += "\"], [\"";
  keyboardJson += SHOW_SOIL_MOISTURE_REPLY;
  keyboardJson += "\"], [\"";
  keyboardJson += alertOn ? "Subscribe" : "Unsubscribe";
  keyboardJson += "\"]]";
  const bool resize = true;
  const bool oneTime = false;
  const bool selective = false;
  bot.sendMessageWithReplyKeyboard(chatId, "Choose from one of the following options", "",
                                   keyboardJson, resize, oneTime, selective);
}