#ifndef _WRITE_STREAM_
#define _WRITE_STREAM_

#include <Arduino.h>
#include "./debug.h"
#include "./timer.h"

const unsigned char ZERO = 0x00;
const unsigned char ONE = 0x01;
const int binaryBufferSize = 16;

class WriteStream: public Timer {
  unsigned char pin;

  public:
    unsigned char* buffer;
    unsigned char binaryBuffer[binaryBufferSize];

    int readIndex = 0;
    int writeIndex = 0;

    void setPin(unsigned char _pin) {
      pin = _pin;
    }

    void reset() {
      readIndex = 0;
      writeIndex = 0;
      buffer = 0;
    }

    void tick() {
      if (readIndex >= binaryBufferSize) {
        readIndex = 0;
        fillBuffer();
      }

      digitalWrite(pin, binaryBuffer[readIndex++]);

      /*if ((readIndex == 0 && writeIndex == 0) ||
        (readIndex > writeIndex && readIndex - writeIndex > 8)) {
        decodeNext();
      }

      //DEBUG("stream wrt %d, read at %d, %d\n", writeIndex, readIndex, buffer[readIndex]);
      digitalWrite(pin, binaryBuffer[readIndex]);
      readIndex++;

      if (readIndex >= binaryBufferSize) {
        readIndex = 0;
      }*/
    }

    void fillBuffer() {
      int i;
      for (i = 0; i < binaryBufferSize / 8; i++) {
        decodeNext(i * 8);
      }
    }

    void write(unsigned char* bytes) {
      buffer = bytes;
      start();
    }

    void decodeNext(int startingPosition) {
      int i, j;
      const char nextByte = *buffer;

      if (nextByte == 0x00) {
        DEBUG("write stream halted\n");
        stop();
        return;
      }

      DEBUG("Next byte: %c\n", nextByte);

      for (i = 7; i >= 0; i--) {
        j = startingPosition + (7 - i);
        binaryBuffer[j] = ZERO;

        if (nextByte & (1 << i)) {
          binaryBuffer[j] = ONE;
        };

        DEBUG("buffer %d at %d \n", binaryBuffer[j], j);

        // writeIndex++;

        // if (writeIndex >= binaryBufferSize) {
          // writeIndex = 0;
        // }
      }

      buffer++;
    }
};

#endif