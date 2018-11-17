#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "./debug.h"

typedef void (*OnConnectingHandler)();

class WifiController {
  public:
    ESP8266WiFiMulti wifi;
    OnConnectingHandler onConnectingCb = NULL;

    WifiController(const char* SSID, const char* key) {
      this->setup(SSID, key);
    }

    void setup(const char* SSID, const char* key) {
      DEBUG.printf("Connect WiFI to %s %s", SSID, key);
      DEBUG.println();

      this->wifi.addAP(SSID, key);
    }

    void connect() {
      while (this->wifi.run() != WL_CONNECTED) {
        DEBUG.write(".");
        delay(100);

        if (this->onConnectingCb != NULL) {
          this->onConnectingCb();
        }
      }

      DEBUG.println();
    }

    void onConnecting(OnConnectingHandler handler) {
      this->onConnectingCb = handler;
    }
};