#ifndef _STREAM_READER_
#define _STREAM_READER_

#ifndef DEBUG
#define DEBUG(...)
#endif

// typedef unsigned int size_t;

#ifndef _ASSERT_CPP_
#include <stdlib.h>
#include <string.h>
#endif

void copyBytes(unsigned char* target, unsigned char* source, size_t amount) {
  for (int i = 0; i < amount; i++) {
    target[i] = source[i];
  }
}

class StreamReader {
  public:
    unsigned char *stream;

  StreamReader(unsigned char *_stream):
      stream(_stream) {}

    unsigned char* readString() {
      int size = strlen((const char*)stream) + 1;
      unsigned char* chars = (unsigned char*) malloc(size);
      copyBytes(chars, stream, size);
      stream += size;

      DEBUG("Read string %s\n", chars);
      return chars;
    }

    char readByte() {
      unsigned char number = (unsigned char)*stream;
      stream++;

      DEBUG("Read char %d\n", number);
      return number;
    }

    bool readBool() {
      char number = (char)*stream;
      stream++;
      bool value = number == 0x01;

      DEBUG("Read bool %d\n", value);
      return value;
    }

    int readLong() {
      unsigned char* bytes = (unsigned char*) malloc(5);
      bytes[4] = 0x00;
      copyBytes(bytes, stream, 4);
      stream += 4;
      long number = strtoul((const char*)bytes, nullptr, 16);
      free(bytes);

      DEBUG("Read long %ld from %s\n", number, bytes);
      return number;
    }
};

#endif