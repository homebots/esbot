#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include "./lib/socket-controller.cpp"
#include "./lib/wifi-controller.cpp"
#include "./lib/bot.cpp"
#include "./debug.h"

WebSocketsClient webSocket;
SocketController socketCtrl(&webSocket);
WifiController wifiCtrl("Mate_Amargo", "ComAguaQuent3");

void setup() {
	DEBUG_OUTPUT.begin(115200);

	for(uint8_t t = 4; t > 0; t--) {
		DEBUG("[SETUP] BOOT WAIT %d...\n", t);
		DEBUG_OUTPUT.flush();
		delay(1000);
	}

  wifiCtrl.onConnecting([]() {
    DEBUG(".");
  });

  wifiCtrl.connect();
  socketCtrl.setup("192.168.1.103", 80);

  socketCtrl.onMessage([](uint8_t* message) {
    DEBUG("%s", message);
    Instruction* i = parseInstruction(message);

    DEBUG("%d", i->id);
    i->run();

    char* output;

    if (i->id == BiRead) {
      snprintf(output, 2, "%d%d", BiRead, i->digitalOutput);
      webSocket.sendBIN((uint8_t*)output, strlen(output));
    }

    if (i->id == BiAnalogRead) {
      snprintf(output, 2, "%d%ld", BiRead, i->analogOutput);
      webSocket.sendBIN((uint8_t*)output, strlen(output));
    }
  });
}

void loop() {
	socketCtrl.loop();
}
