#include "./instruction.h"

class WriteInstruction: public Instruction {
  unsigned char pin;
  int value;

  public:
    WriteInstruction(unsigned char _pin, int _value):
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