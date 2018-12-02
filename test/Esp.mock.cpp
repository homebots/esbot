class EspMock {
  public:
    bool wasRestarted;

    void restart() {
      wasRestarted = true;
    }
};

EspMock ESP;