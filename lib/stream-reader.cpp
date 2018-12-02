#ifndef DEBUG
#define DEBUG(...)
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

    int readByte() {
      int number = (int)*stream;
      stream++;

      DEBUG("Read char %d\n", number);
      return number;
    }

    bool readBool() {
      char number = (char)*stream++;
      bool value = number == 0x01;

      DEBUG("Read bool %d\n", value);
      return value;
    }

    int readNumber() {
      unsigned char* bytes = (unsigned char*) malloc(5);
      *bytes = 0x30;
      bytes[4] = 0x00;
      copyBytes(bytes + 1, stream, 3);
      stream += 3;
      long number = strtol((const char*)bytes, nullptr, 16);
      free(bytes);

      DEBUG("Read number %ld\n", number);
      return number;
    }

    int readLong() {
      unsigned char* bytes = (unsigned char*) malloc(5);
      bytes[4] = 0x00;
      copyBytes(bytes, stream, 4);
      stream += 4;
      long number = strtol((const char*)bytes, nullptr, 16);
      free(bytes);

      DEBUG("Read long %ld\n", number);
      return number;
    }
};
