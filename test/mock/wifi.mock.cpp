const int WL_CONNECTED = 1;

class ESP8266WiFiMulti {
  public:
  void addAP(const char* ssid, const char* key) {}

  int run() {
    return WL_CONNECTED;
  }
};