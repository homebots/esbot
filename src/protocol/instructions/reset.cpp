#include "./instruction.h"

class ResetInstruction: public Instruction {
  public:
    void run() {
      DEBUG("Restart ESP");
      ESP.restart();
    }
};
