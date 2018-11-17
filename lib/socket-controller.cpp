#include <Arduino.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include "./debug.h"

const char* SERVER_URL = "192.168.1.103";

typedef void (*SocketConnectHandler)(WebSocketsClient webSocket);
typedef void (*SocketMessageHandler)(WebSocketsClient webSocket, uint8_t* payload, size_t length);

SocketConnectHandler _onConnectCb = NULL;
SocketConnectHandler _onDisconnectCb = NULL;
SocketMessageHandler _onMessageCb = NULL;

class SocketController {
  public:
    WebSocketsClient webSocket;

    SocketController() {}

    void onConnect(SocketConnectHandler handler) {
      _onConnectCb = handler;
    }

    void onDisconnect(SocketConnectHandler handler) {
      _onDisconnectCb = handler;
    }

    void onMessage(SocketMessageHandler handler) {
      _onMessageCb = handler;
    }

    void setup() {
      DEBUG.println("Start socket");

      this->webSocket.onEvent(
        [&](WStype_t type, uint8_t* payload, size_t length) {
          DEBUG.printf("Socket Event %d", type);
          DEBUG.println();

          if (type == WStype_DISCONNECTED) {
            DEBUG.println("Disconnected");
            if (_onDisconnectCb != NULL) {
              _onDisconnectCb(this->webSocket);
            }
            return;
          }

          hexdump(payload, length);

          if (type == WStype_CONNECTED) {
            DEBUG.println("Connected");
            if (_onConnectCb != NULL) {
              _onConnectCb(this->webSocket);
            }
            return;
          }

          if (type == WStype_TEXT) {
            DEBUG.println("Text received");

            if (_onMessageCb != NULL) {
              _onMessageCb(this->webSocket, payload, length);
            }
          }
        });

      this->webSocket.setReconnectInterval(5000);
    }

    void connect() {
      this->webSocket.begin(SERVER_URL, 80, "/");
    }

    void loop() {
      this->webSocket.loop();
    }
};