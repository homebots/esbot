#include <Arduino.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include "./pin-controller.cpp"
#include "./ticker.cpp"
#include "./debug.h"

typedef void (*SocketMessageHandler)(uint8_t* message);

class SocketController {
  protected:
    WebSocketsClient *webSocket;
    Ticker *pingTicker;
    SocketMessageHandler _onMessage;

  public:
    SocketController(WebSocketsClient *webSocket) {
      Ticker pingTicker(5000);
      this->pingTicker = &pingTicker;
      this->webSocket = webSocket;
    }

    void setup(const char* host, int port) {
      webSocket->begin(host, port, "/");
      webSocket->setReconnectInterval(5000);
      webSocket->onEvent([&](WStype_t type, uint8_t* message, size_t length) {
        DEBUG.printf("[>>>>>> SOCKET] Socket event %d\n", type);

        switch(type) {
          case WStype_DISCONNECTED:
            this->onDisconnect();
            this->pingTicker->stop();
            break;

          case WStype_CONNECTED:
            this->onConnect(message);
            delay(100);
            this->pingTicker->start();
            break;

          case WStype_TEXT:
            this->onMessageReceived(message);
            break;
        }
      });
    }

    void loop() {
      this->webSocket->loop();
      this->pingTicker->loop();
    }

    void onConnect(uint8_t* message) {
      DEBUG.printf("[>>>>>] Connected to %s!\n", message);
      this->webSocket->sendTXT("bot");
    }

    void onDisconnect() {
      DEBUG.printf("[>>>>>] Disconnected!\n");
    }

    void onMessage(SocketMessageHandler handler) {
      this->_onMessage = handler;
    }

  private:
    void onMessageReceived(uint8_t* message) {
      DEBUG.printf("[>>>>>] message: %s\n", message);
      this->_onMessage(message);
    }

    void ping() {
      this->webSocket->sendTXT("bot ping");
    }
};