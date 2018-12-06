#include "./instruction.h"

class DebugInstruction: public Instruction {
  bool enabled;
  public:
    DebugInstruction(bool _enabled):
      enabled(_enabled) {}

    void run() {
      if (enabled) {
        Serial.begin(115200);
      } else {
        Serial.end();
      }
    }
};