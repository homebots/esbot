#include "src/main.cpp";

MainController ctrl;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  ctrl.setup();
}

void loop() {
  ctrl.loop();
}