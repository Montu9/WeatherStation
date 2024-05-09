#include "bot.h"

Bot::Bot(const String& token, WiFiClientSecure& client, String chatId,
         SensorsReader* sensorsReader)
    : bot(token, client), chatId(chatId) {
  // Setup Telegram certificate
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  bot.setMyCommands(F("["
    "{\"command\":\"start\",       \"description\":\"Learn more about bot\"},"
    "{\"command\":\"subscribe\",   \"description\":\"Start receiving measurement updates\"},"
    "{\"command\":\"unsubscribe\", \"description\":\"Stop receiving measurement updates\"},"
    "{\"command\":\"state\",       \"description\":\"Request current sensor measurements\"},"
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
    int newMessageCount = bot.getUpdates(bot.last_message_received + 1);
  }

  // Revert to original long poll
  bot.longPoll = originalLongPoll;
}

void Bot::writeAlert() {
  if (!alertOn) {
    return;
  }

  String message = sensorsReader->toString(sensorsReader->readData());
  message += "\nYou are receiving this message because you have subscribed to this topic.\n";
  message += "To unsubscribe type /unsubscribe\nTo see all available commands type /help";
  Serial.printf("Bot :: Writing alert: %s\n", message.c_str());
  bot.sendMessage(chatId, message, "");
}

void Bot::setLongPoll(int longPoll) {
  bot.longPoll = longPoll;
}

void Bot::handleMessages(int messageCount) {
  Serial.println("Bot :: handleMessages");
  Serial.println(String(messageCount));

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

    String output = "";

    if (text == "/start") {
      output = "Welcome, " + from + ".\n";
      output += "I'm Evergreen bot\n";
      output += "What would you like to know?\n";
      output += "Type /help to see available commands\n";
    } else if (text == "/subscribe") {
      alertOn = true;
      lastTimeBotAlert = millis(); // Reset alert timer
      output = "You have subscribed to measurement updates";
    } else if (text == "/unsubscribe") {
      alertOn = false;
      output = "You have unsubscribed to measurement updates";
    } else if (text == "/state") {
      output = sensorsReader->toString(sensorsReader->readData());
      output += "\nYou are currently ";
      output += alertOn ? "subscribed" : "unsubscribed";
    } else if (text == "/help") {
      output = "Welcome, " + from + ".\n";
      output += "I'm Evergreen bot\n";
      output += "Use the following commands to control the LED.\n\n";
      output += "/start to learn more about bot\n";
      output += "/subscribe to start receiving measurement updates\n";
      output += "/unsubscribe to stop receiving measurement updates\n";
      output += "/state to request current sensor measurements\n";
      output += "/help to see available commands";
    } else {
      output = "Unknown command. Please type /help to see available commands";
    }

    Serial.printf("Bot :: Sending to user: %s\n", output.c_str());
    bot.sendMessage(currentChatId, output, "");
  }
}