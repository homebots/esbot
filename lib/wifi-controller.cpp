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
      DEBUG("Connect WiFI to %s %s", SSID, key);
      DEBUG("\n");

      this->wifi.addAP(SSID, key);
    }

    void connect() {
      while (this->wifi.run() != WL_CONNECTED) {
        DEBUG(".");
        delay(100);

        if (this->onConnectingCb != NULL) {
          this->onConnectingCb();
        }
      }

      DEBUG("\n");
    }

    void onConnecting(OnConnectingHandler handler) {
      this->onConnectingCb = handler;
    }
};