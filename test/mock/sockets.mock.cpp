
#define WEBSOCKETSCLIENT_H_

typedef enum {
    WStype_ERROR,
    WStype_DISCONNECTED,
    WStype_CONNECTED,
    WStype_TEXT,
    WStype_BIN,
} WStype_t;

typedef std::function<void (WStype_t type, uint8_t * payload, size_t length)> WebSocketClientEvent;

class WebSocketsClient {
  WebSocketClientEvent _cb;

  public:
    unsigned char* lastBIN;

    void begin(const char* host, int port, const char* path) {}
    void setReconnectInterval(int interval) {}
    void onEvent(WebSocketClientEvent cb) {
      _cb = cb;
    }

    void loop() {}

    void sendTXT(unsigned char* bytes) {}
    void sendBIN(unsigned char* bytes, int length) {
      lastBIN = bytes;
    }
};