#include <Arduino.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

int readCommand(uint8_t* payload) {
  char *result;
  int instruction;

  instruction = strtol((char*)payload, &result, 16);
  printf("%d [%s]", instruction, result);
}
