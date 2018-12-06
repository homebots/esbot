#include "./instruction.h"

class ReadInstruction: public Instruction {
  unsigned char pin;
  unsigned char isAnalog;

  public:
    ReadInstruction(unsigned char _pin, bool _isAnalog):
      pin(_pin), isAnalog(_isAnalog), callback(cb) {}

    void run() {
      pinMode(pin, INPUT);

      if (isAnalog) {
        callback(analogRead(pin));
      } else {
        callback(digitalRead(pin));
      }
    }
};