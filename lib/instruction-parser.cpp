#include <Arduino.h>
#include "./instruction.h";
#include "./debug.h";

Instruction noop(DoNoop, 0, 0);

class InstructionParser {
  public:
    Instruction parse(uint8_t* message) {
      unsigned char* header = readBytes(message, 0, 2);
      uint8_t* payload = message + 4;

      if (header == NULL) {
        return noop;
      }

      DEBUG.printf("op %d - pin %d - payload %s", header[0], header[1], payload);

      Instruction instruction(
        (InstructionType) header[0],
        (uint8_t)header[1],
        (uint8_t*)payload
      );

      return instruction;
    }

};