#ifndef _STREAM_WRITER_H_
#define _STREAM_WRITER_H_

#include "./debug.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct StreamNode {
  unsigned char value = 0;
  StreamNode* next = 0;
  StreamNode* previous = 0;
};

class StreamEncoder {
  StreamNode* head;
  StreamNode* current;

    void reset();
  public:
    int length = 0;
    StreamEncoder();

    void writeByte(unsigned char value);
    void writeBool(bool value);
    void writeNumber(int value);
    void writeString(const char* value);
    void push(StreamNode* next);
    unsigned char* getStream();
    void discardStream();
};

#endif
