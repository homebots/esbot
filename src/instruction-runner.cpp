#ifndef _INSTRUCTIONS_
#define _INSTRUCTIONS_

#ifndef DEBUG
#define DEBUG(...)
#endif

#include <Arduino.h>
#include <WebSocketsClient.h>
#include "./stream-reader.cpp"
#include "./stream-writer.cpp"
// #include "./timer.cpp"

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

class InstructionRunner {
  public:
    WebSocketsClient* webSocket;
    StreamWriter output;

    void setSocket(WebSocketsClient* socket) {
      webSocket = socket;
    }

    void run(unsigned char* byteStream) {
      StreamReader reader(byteStream);
      char id = reader.readByte();

      switch (id) {
        case BiNoop:
          this->noop();
          break;

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
            reader.readBool()
          );
          break;

        case BiDebug:
          this->debug(
            reader.readBool()
          );
          break;
      }
    }

    void noop() {
      output.writeByte(BiNoop);
      output.writeByte(0x01);
      sendOutput();
    }

    void reset() {
      output.writeByte(BiReset);
      output.writeByte(0x01);

      sendOutput();
      ESP.restart();
    }

    void debug(bool enabled) {
      output.writeByte(BiDebug);

      if (enabled) {
        Serial.begin(115200);
        output.writeByte(0x01);
      } else {
        Serial.end();
        output.writeByte(0x00);
      }

      sendOutput();
    }

    void readPin(unsigned char pin, unsigned char isAnalog) {
      pinMode(pin, INPUT);

      if (isAnalog) {
        readAnalogPin(pin);
      } else {
        readDigitalPin(pin);
      }
    }

    void readDigitalPin(unsigned char pin) {
      output.writeByte(BiRead);
      output.writeBool(false);
      output.writeByte(digitalRead(pin));

      sendOutput();
    }

    void readAnalogPin(unsigned char pin) {
      output.writeByte(BiRead);
      output.writeBool(true);
      output.writeNumber(analogRead(pin));

      sendOutput();
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

    void loop() {}

  private:
    void sendOutput() {
      unsigned char* bytes = output.getStream();
      webSocket->sendBIN(bytes, strlen((const char*)bytes));
    }

};

#endif