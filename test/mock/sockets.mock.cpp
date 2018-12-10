typedef enum {
    WStype_ERROR,
    WStype_DISCONNECTED,
    WStype_CONNECTED,
    WStype_TEXT,
    WStype_BIN,
} WStype_t;

typedef void (*OnEventCallback)(WStype_t type, uint8_t * payload, size_t length);

class WebSocketsClient {
  OnEventCallback _cb;

  public:
    void begin(const char* host, int port, const char* path) {}
    void setReconnectInterval(int interval) {}
    void onEvent(OnEventCallback cb) {
      _cb = cb;
    }

    void loop() {}

    void sendTXT(unsigned char* bytes) {}
    void sendBIN(unsigned char* bytes, int length) {}
};