// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <functional>
// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266WiFiMulti.h>
// #include <WebSocketsClient.h>

#include "src/main.cpp";

MainController ctrl;

void setup() {
  ctrl.setup();
}

void loop() {
  ctrl.loop();
}