#ifndef _READ_STREAM_
#define _READ_STREAM_

#include <Arduino.h>
#include <os_type.h>
#include "./timer.h"
#include "./stream-encoder.h"

class ReadStream: public Timer {
  bool isStreaming = false;
  bool isAnalog = false;
  int bufferSize = 0;
  unsigned char pin;

  public:
    StreamEncoder output;

    void setPin(unsigned char _pin) {
      pin = _pin;
    }

    void setAnalog(bool value) {
      isAnalog = value;
    }

    void setBufferSize(unsigned long size) {
      bufferSize = size;
    }

    void reset(unsigned char firstByte) {
      output.discardStream();
      output.writeByte(firstByte);
    }

    void tick() {
      if (isAnalog) {
        output.writeByte(analogRead(pin));
        return;
      }

      output.writeByte(digitalRead(pin));
    }

    bool isFull() {
      return running && bufferSize && output.length >= bufferSize;
    }

    unsigned long getLength() {
      return output.length;
    }

    unsigned char* getBuffer() {
      return output.getStream();
    }
};

#endif