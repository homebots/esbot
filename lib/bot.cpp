#include "./stream-reader.cpp"
#include "./instruction.cpp"

typedef enum {
  BiNoop = 0x01,
  BiReset = 0x02,
  BiPing = 0x03,
  BiWrite = 0x04,
  BiRead = 0x05,
  BiAnalogWrite = 0x06,
  BiAnalogRead = 0x07
} BotInstruction;

Instruction* parseInstruction(unsigned char* byteStream) {
  StreamReader reader(byteStream);
  char id = reader.readByte();
  Instruction* i = new NoopInstruction;

  if (id == BiReset) {
    i = new ResetInstruction;
  }

  if (id == BiWrite) {
    i = new WriteInstruction(
      reader.readByte(),
      reader.readBool()
    );
  }

  if (id == BiRead) {
    i = new ReadInstruction(
      reader.readByte(),
      false
    );
  }

  if (id == BiAnalogWrite) {
    i = new AnalogWriteInstruction(
      reader.readByte(),
      reader.readNumber()
    );
  }

  if (id == BiAnalogRead) {
    i = new ReadInstruction(
      reader.readByte(),
      true
    );
  }

  i->id = id;

  return i;
}
