#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include "./lib/socket-controller.cpp"
#include "./lib/wifi-controller.cpp"
#include "./lib/instruction-parser.cpp"
#include "./debug.h"

WebSocketsClient webSocket;
SocketController socketCtrl(&webSocket);
WifiController wifiCtrl("Mate_Amargo", "ComAguaQuent3");
PinController pin0(0);
PinController pin2(2);
InstructionParser parser;

void setup() {
	DEBUG.begin(115200);

	for(uint8_t t = 4; t > 0; t--) {
		DEBUG.printf("[SETUP] BOOT WAIT %d...\n", t);
		DEBUG.flush();
		delay(1000);
	}

  wifiCtrl.onConnecting([]() {
    DEBUG.write(".");
  });

  wifiCtrl.connect();
  socketCtrl.setup("192.168.1.103", 80);

  socketCtrl.onMessage([](uint8_t* message) {
    Instruction i = parser.parse(message);
    long value = -1;
    char* result;
    PinController* pin;

    switch(i.instruction) {
      case DoNoop:
        break;

      case DoRead:
        break;
    }
}

void loop() {
	socketCtrl.loop();
}
