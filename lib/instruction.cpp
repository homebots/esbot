#ifndef DEBUG
#define DEBUG(...)
#endif

class Instruction {
  public:
    unsigned char id;
    unsigned char digitalOutput;
    int analogOutput;
    virtual void run() {}
};

class NoopInstruction: public Instruction {
  public:
    void run() {}
};

class ResetInstruction: public Instruction {
  public:
    void run() {
      DEBUG("Restart ESP");
      ESP.restart();
    }
};

class WriteInstruction: public Instruction {
  unsigned char pin;
  unsigned char value;

  public:
    WriteInstruction(unsigned char _pin, bool _value):
      pin(_pin), value(_value) {}

    void run() {
      pinMode(pin, OUTPUT);

      if (value) {
        digitalWrite(pin, HIGH);
        return;
      }

      digitalWrite(pin, LOW);
    }
};

class AnalogWriteInstruction: public Instruction {
  unsigned char pin;
  unsigned char value;

  public:
    AnalogWriteInstruction(unsigned char _pin, unsigned char _value):
      pin(_pin), value(_value) {}

    void run() {
      pinMode(pin, OUTPUT);
      analogWrite(pin, value);
    }
};

class ReadInstruction: public Instruction {
  unsigned char pin;
  unsigned char isAnalog;

  public:
    ReadInstruction(unsigned char _pin, bool _isAnalog):
      pin(_pin), isAnalog(_isAnalog) {}

    void run() {
      pinMode(pin, INPUT);

      if (isAnalog) {
        analogOutput = analogRead(pin);
      } else {
        digitalOutput = digitalRead(pin);
      }
    }
};
