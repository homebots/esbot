#include <Arduino.h>
#include <string.h>
#include <WebSocketsClient.h>
#include "./lib/pin-controller.cpp"
#include "./lib/socket-controller.cpp"
#include "./lib/wifi-controller.cpp"
#include "./debug.h"

PinController pin2(2);
SocketController socketCtrl;
WifiController wifiCtrl("Mate_Amargo", "ComAguaQuent3");

void setup() {
  DEBUG.begin(115200);
  // DEBUG.setDebugOutput(true);
  for(uint8_t t = 4; t > 0; t--) {
    DEBUG.printf("[SETUP] BOOT WAIT %d...\n", t);
    DEBUG.flush();
    delay(1000);
  }

  pin2.toOutput();
  pin2.blink(3, 100);

  wifiCtrl.onConnecting([]() {
    pin2.blinkFade((uint8_t)1, (uint8_t)16);
  });

  socketCtrl.onConnect([](WebSocketsClient webSocket) {
    webSocket.sendTXT("bot");
    pin2.blink(3, 250);
  });

  socketCtrl.onDisconnect([](WebSocketsClient webSocket) {
    pin2.blinkFade(2, 32);
  });

  socketCtrl.onMessage([](WebSocketsClient webSocket, uint8_t* payload, size_t length) {
    webSocket.sendTXT("OK");
    pin2.blink(3, 250);
  });

  wifiCtrl.connect();
  socketCtrl.setup();
  socketCtrl.connect();
}

void loop() {
  socketCtrl.loop();
}
