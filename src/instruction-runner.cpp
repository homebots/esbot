#ifndef _INSTRUCTIONS_
#define _INSTRUCTIONS_

#include <Arduino.h>
#include <WebSocketsClient.h>
#include "./stream-reader.h"
#include "./stream-encoder.h"
#include "./read-stream.cpp"
#include "./write-stream.cpp"

typedef enum {
  BiNoop = 0x01,
  BiReset = 0x02,
  BiDebug = 0x03,
  BiPing = 0x04,
  BiWrite = 0x05,
  BiAnalogWrite = 0x06,
  BiRead = 0x07,
  BiReadStream = 0x08,
  BiWriteStream = 0x09,
  BiStopReadStream = 0x0a,
  BiStopWriteStream = 0x0b
} InstructionCode;

class InstructionRunner {
  public:
    WebSocketsClient* webSocket;
    StreamEncoder output;
    ReadStream readStream;
    WriteStream writeStream;
    unsigned char* uid;

    void setSocket(WebSocketsClient* socket) {
      webSocket = socket;
    }

    void setUid(unsigned char* uniqueId) {
      uid = uniqueId;
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

        case BiDebug:
          this->debug(
            reader.readBool()
          );
          break;

        case BiPing:
          this->pong();
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

        case BiReadStream:
          this->startReadStream(
            reader.readByte(),
            reader.readLong(),
            reader.readLong()
          );
          break;

        case BiStopReadStream:
          this->stopReadStream();
          break;

        case BiStopWriteStream:
          this->stopWriteStream();
          break;

        case BiWriteStream:
          this->startWriteStream(
            reader.readByte(),
            reader.readLong(),
            byteStream + 6
          );
          break;
      }
    }

    void noop() {
      output.writeByte(BiNoop);
      output.writeByte(0x01);
      sendOutput();
    }

    void pong() {
      output.writeByte(BiPing);
      output.writeString((const char*)uid);
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
        analogWrite(pin, constrain(value, 1, PWMRANGE));
        return;
      }

      if (value > 0) {
        digitalWrite(pin, HIGH);
        return;
      }

      digitalWrite(pin, LOW);
    }

    void startReadStream(unsigned char pin, unsigned long frequency, unsigned long bufferSize) {
      pinMode(pin, INPUT);
      readStream.setPin(pin);
      readStream.setInterval(frequency);
      readStream.setBufferSize(bufferSize);
      readStream.start();
    }

    void stopReadStream() {
      readStream.stop();
      readStream.reset(BiReadStream);
    }

    void stopWriteStream() {
      writeStream.stop();
      writeStream.reset();
    }

    void startWriteStream(unsigned char pin, unsigned long frequency, unsigned char* bytes) {
      writeStream.setPin(pin);
      writeStream.setInterval(frequency);
      writeStream.reset();
      writeStream.write(bytes);
    }

    void loop() {
      writeStream.loop();
      readStream.loop();

      if (readStream.isFull()) {
        unsigned long length = readStream.getLength();
        webSocket->sendBIN(readStream.output.getStream(), length);
        readStream.reset(BiReadStream);
      }
    }

  private:
    void sendOutput() {
      unsigned char* bytes = output.getStream();
      webSocket->sendBIN(bytes, strlen((const char*)bytes));
      free(bytes);
    }

};

#endif