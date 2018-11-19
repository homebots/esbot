#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "./debug.h";

typedef enum {
  DoNoop =          0x00,
  DoReset =         0x01,
  DoRead =          0x02,
  DoWrite =         0x03,
  DoAnalogRead =    0x04,
  DoAnalogWrite =   0x05,
} InstructionType;

/**
 * Instruction id
 * mode
 * pin
 * value
 */
class Instruction {
  public:
    InstructionType instruction;
    uint8_t pin;
    uint8_t* value;

    Instruction(InstructionType id, uint8_t _pin, uint8_t* _value):
      instruction(id),
      pin(_pin),
      value(_value) {}
};

/**
 * char* bytes = readBytes("0011ff", 0, 3);
 */
unsigned char* readBytes(unsigned char* hexString, int startAt, int bytesToRead) {
  int hexLength = strlen((const char*) hexString);
  int lengthToRead = startAt + bytesToRead * 2;

  if (hexLength % 2 == 1) {
    printf("Invalid hex string to read: %s\n", lengthToRead, hexString);
    return NULL;
  }

  if (lengthToRead > hexLength) {
    printf("Not enough bytes (%d) to read in %s\n", lengthToRead, hexString);
    return NULL;
  }

  int i = 0;
  int start;
  unsigned char* output = (unsigned char*) malloc(bytesToRead);
  char hexPair[3];
  hexPair[3] = '\0';

  // from startAt + i, 2 bytes
  for (; i < bytesToRead; i++) {
    start = startAt + i * 2;
    hexPair[0] = hexString[start];
    hexPair[1] = hexString[start + 1];

    output[i] = strtoul(hexPair, nullptr, 16);
  }

  return output;
}

#endif