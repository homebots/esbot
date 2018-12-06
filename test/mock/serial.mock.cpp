class SerialMock {
  public:
    int rate = 0;

    void begin(int _rate) {
      rate = _rate;
    }

    void end() {
      rate = 0;
    }
};

SerialMock Serial;