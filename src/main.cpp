#ifndef _BOT_PROTOCOL_
#define _BOT_PROTOCOL_

#ifndef DEBUG
#define DEBUG(...)
#endif

#ifndef _ASSERT_CPP_
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#endif

#include "./debug.h"
#include "./instruction-runner.cpp"

class MainController {
  public:
    WebSocketsClient webSocket;
    ESP8266WiFiMulti wifi;
    InstructionRunner instructions;

  void setup() {
    this->setupWifi();
    this->setupWebSocket();
  }

  void loop() {
    webSocket.loop();
    instructions.loop();
  }

  void setupWebSocket() {
    webSocket.begin("37.139.28.195", 2021, "/z");
    webSocket.setReconnectInterval(5000);

    webSocket.onEvent([&](WStype_t type, uint8_t* message, size_t length) {
      switch(type) {
        case WStype_CONNECTED:
          DEBUG("> %s:2021/z\n", message);
          delay(100);
          sendIdentity();
          break;

        case WStype_BIN:
          onMessageReceived(message);
          break;

        default:
          break;
      }
    });

    instructions.setSocket(&webSocket);
  }

  void setupWifi() {
    wifi.addAP("HomeBots", "HomeBots");

#ifdef WIFI_SSID
    wifi.addAP(WIFI_SSID, WIFI_KEY);
#endif

    while (this->wifi.run() != WL_CONNECTED) {
      DEBUG(".");
      delay(100);
    }

    DEBUG("Connected to WiFi\n");
  }

  void sendIdentity() {
    // unsigned char macAddress[18];
    // wifi.macAddress().getBytes(macAddress, 18);
    // webSocket.sendTXT(strcat((char*) "bot::", (char*) macAddress));
  }

  void onMessageReceived(uint8_t* stream) {
    instructions.run(stream);
  }
};

#endif