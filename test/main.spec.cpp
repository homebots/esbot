// #define DEBUG(...) printf(__VA_ARGS__);

#include "assert.h"
#include <string.h>
#include "mock/arduino.mock.cpp"
#include "mock/ESP.mock.cpp"
#include "mock/serial.mock.cpp"
#include "mock/sockets.mock.cpp"
#include "mock/time.mock.cpp"
#include "mock/wifi.mock.cpp"

#include "main.cpp"

int main() {
  describe("Main controller", []{
    it("should set up the wifi", [] {
      MainController main;
      main.setup();
    });
  });
  testSummary();
}