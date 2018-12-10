#include "./loop.cpp"
#include "./protocol/main.cpp"
#include "./debug.h"

// #ifndef WIFI_SSID
// const char WIFI_SSID = ""
// const char WIFI_KEY = ""
// #endif

class MainController: public LoopCallable {
  public:
    WebSocketsClient webSocket;
    ESP8266WiFiMulti wifi;
    Instructions input;

  void setup() {
    this->setupWifi();
    this->setupWebSocket();

    GlobalLoop.add(this);
  }

  void loop() {
    webSocket.loop();
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
      }
    });
  }

  void setupWifi() {
    wifi.addAP("HomeBots", "HomeBots");
    wifi.addAP(WIFI_SSID, WIFI_KEY);

    while (this->wifi.run() != WL_CONNECTED) {
      DEBUG(".");
      delay(100);
    }

    DEBUG("Connected to WiFi\n");
  }

  void sendIdentity() {
    unsigned char macAddress[18];
    WiFi.macAddress().getBytes(macAddress, 18);
    webSocket.sendTXT(unsigned char*) (strcat((char*) "bot::", (char*) macAddress)));
  }

  void onMessageReceived(uint8_t* message) {
    class Output: public Callable {
      WebSocketsClient* s;
      Output(WebSocketsClient* socket):
        s(socket) {}

      void call(unsigned char instruction, unsigned char* bytes) {
        switch (instruction) {
          default:
            s.sendBIN((uint8_t*)bytes, strlen((const char*)bytes));
        }
      }
    }

    input.parseInstruction(stream, output);
  }
};