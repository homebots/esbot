#include <stdio.h>

struct StreamNode {
  unsigned char value = 0;
  StreamNode* next = 0;
  StreamNode* previous = 0;
};

class StreamWriter {
  int length = 0;
  StreamNode* head;
  StreamNode* current;

  public:
    StreamWriter() {
      head = new StreamNode;
      current = head;
    }

    void writeByte(unsigned char value) {
      StreamNode* next = new StreamNode;
      next->value = value;
      next->next = 0;

      push(next);
    }

    void writeBool(bool value) {
      writeByte(value == true ? 0x30 : 0x31);
    }

    void writeNumber(int value) {
      if (value > 65535) {
        value = 65535;
      }

      char hexNumber[5];
      sprintf((char*)&hexNumber, "%x", value);
      int paddingSize = 4 - strlen(hexNumber);

      char buffer[9] = "0000";
      char output[5] = "0000";
      strcat((char*) &buffer, (const char*)hexNumber);

      int i;
      int j = 3;
      int start = 7 - paddingSize;
      int end = 4 - paddingSize;

      for (i = start; i >= end; i--) {
        output[j--] = buffer[i];
      }

      for (i = 0; i < 4; i++) {
        writeByte(output[i]);
      }
    }

    void writeString(const char* value) {
      int i = 0;
      int length = strlen(value);

      for (; i < length; i++) {
        writeByte(value[i]);
      }
    }

    void push(StreamNode* next) {
      // DEBUG(">> %ld %c\n", next, next->value);
      next->previous = current;
      current->next = next;
      current = next;
      length++;
    }

    unsigned char* getStream() {
      unsigned char* bytes = (unsigned char*) malloc(length + 1);
      bytes[length] = '\0';

      int index = 0;
      StreamNode* node = (StreamNode*)head;

      while (node->next != 0) {
        node = node->next;
        bytes[index++] = node->value;
        delete node->previous;
      }

      // DEBUG("%d> %s\n", length, bytes);

      return bytes;
    }
};