#ifndef DEBUG
#define DEBUG(...)
#endif

#include "callable.h"
#include <stdlib.h>

using namespace std;

// typedef std::function<void (WStype_t type, uint8_t * payload, size_t length)> Foo;
// typedef void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length);

class InstructionResponse: public Callable {
  public:
    virtual void call(unsigned char instructionId, unsigned char* output) {}
};

class InstructionLongResponse: public InstructionResponse {
  public:
    virtual void call(unsigned char instructionId, unsigned long output) {}
};


typedef enum {
  BiNoop = 0x01,
  BiReset = 0x02,
  BiPing = 0x03,
  BiWrite = 0x04,
  BiRead = 0x05,
  BiAnalogWrite = 0x06,
  BiReadStream = 0x07,
  BiWriteStream = 0x08,
  BiStopStream = 0x09,
  BiDebug = 0x0a,
} InstructionCode;


class Instructions {
  public:

    void parseInstruction(unsigned char* byteStream, InstructionResponse* callback) {
      StreamReader reader(byteStream);
      char id = reader.readByte();

      switch (id) {
        case BiReset:
          this->reset();
          break;

        case BiWrite:
        case BiAnalogWrite:
          this->writePin(
            reader.readByte(),
            (id == BiAnalogWrite),
            (id == BiAnalogWrite) ? reader.readLong() : reader.readBool()
          );
          break;

        case BiRead:
          this->readPin(
            reader.readByte(),
            reader.readBool(),
            callback
          );
          break;

        case BiDebug:
          this->debug(
            reader.readBool()
          );
          break;
      }
    }

    void reset() {
      ESP.restart();
    }

    void debug(bool enabled) {
      if (enabled) {
        Serial.begin(115200);
      } else {
        Serial.end();
      }
    }

    void readPin(unsigned char pin, unsigned char isAnalog, InstructionResponse* callback) {
      pinMode(pin, INPUT);

      if (isAnalog) {
        readAnalogPin(pin, (InstructionLongResponse*)callback);
      } else {
        unsigned char value = digitalRead(pin);
        callback->call(BiRead, &value);
      }
    }

    void readAnalogPin(unsigned char pin, InstructionLongResponse* callback) {
      unsigned long value = analogRead(pin);
      callback->call(BiRead, value);
    }

    void writePin(unsigned char pin, bool isAnalog, long value) {
      pinMode(pin, OUTPUT);
      DEBUG("Write %d %d", pin, value);

      if (isAnalog) {
        analogWrite(pin, value);
        return;
      }

      if (value > 0) {
        digitalWrite(pin, HIGH);
        return;
      }

      digitalWrite(pin, LOW);
    }
};