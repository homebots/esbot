#ifndef _STREAM_WRITER_
#define _STREAM_WRITER_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "./stream-encoder.h"
#include "./debug.h"

StreamEncoder::StreamEncoder() {
  reset();
}

void StreamEncoder::writeByte(unsigned char value) {
  StreamNode* next = new StreamNode;
  next->value = value;
  next->next = 0;

  push(next);
}

void StreamEncoder::writeBool(bool value) {
  writeByte(value == true ? 0x30 : 0x31);
}

void StreamEncoder::writeNumber(int value) {
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

void StreamEncoder::writeString(const char* value) {
  int i = 0;
  int length = strlen(value);

  for (; i < length; i++) {
    writeByte(value[i]);
  }
}

void StreamEncoder::push(StreamNode* next) {
  next->previous = current;
  current->next = next;
  current = next;
  length++;
}

unsigned char* StreamEncoder::getStream() {
  unsigned char* bytes = (unsigned char*) malloc(length + 1);
  bytes[length] = '\0';

  int index = 0;
  StreamNode* node = (StreamNode*)head;

  while (node->next != 0) {
    node = node->next;
    bytes[index++] = node->value;
    delete node->previous;
  }

  reset();

  return bytes;
}

void StreamEncoder::discardStream() {
  StreamNode* node = (StreamNode*)head;

  while (node->next != 0) {
    node = node->next;
    delete node->previous;
  }

  reset();
}

void StreamEncoder::reset() {
  head = new StreamNode;
  current = head;
  length = 0;
}

#endif
