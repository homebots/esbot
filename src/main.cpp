#ifndef _BOT_PROTOCOL_
#define _BOT_PROTOCOL_

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
    unsigned char* macAddress = (unsigned char*)malloc(18);
    this->setupWifi();
    this->setupWebSocket();

    WiFi.macAddress().getBytes(macAddress, 18);
    instructions.setUid(macAddress);

    instructions.writePin(0, false, 0);
    instructions.writePin(2, false, 0);
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
          delay(100);
          break;

        case WStype_BIN:
          instructions.run(message);
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
};

#endif