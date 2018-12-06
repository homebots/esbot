#ifndef _BOT_PROTOCOL_
#define _BOT_PROTOCOL_

#ifndef DEBUG
#define DEBUG(...)
#endif

#include "./stream/main.cpp"
#include "./instructions/main.cpp"

typedef enum {
  BiNoop = 0x01,
  BiReset = 0x02,
  BiPing = 0x03,
  BiWrite = 0x04,
  BiRead = 0x05,
  BiAnalogWrite = 0x06,
  BiAnalogRead = 0x07,
  BiDebug = 0x08
} BotInstruction;

class _BotProtocol {
public:
  Instruction* parseInstruction(unsigned char* byteStream, Callback callback = 0) {
    StreamReader reader(byteStream);
    char id = reader.readByte();
    Instruction* i = new NoopInstruction;

    if (id == BiReset) {
      i = new ResetInstruction;
    }

    if (id == BiWrite && id == BiAnalogWrite) {
      i = new WriteInstruction(
        reader.readByte(),
        id == BiAnalogWrite ? reader.readLong() : reader.readBool()
      );
    }

    if (id == BiRead && id == BiAnalogRead) {
      i = new ReadInstruction(
        reader.readByte(),
        id == BiAnalogRead,
        callback
      );
    }

    if (id == BiDebug) {
      i = new DebugInstruction(
        reader.readBool()
      );
    }

    i->id = id;

    return i;
  }
};

_BotProtocol BotProtocol;

#endif