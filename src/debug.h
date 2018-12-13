#ifndef DEBUG
#include <Arduino.h>

#define DEBUG(...) Serial.printf(__VA_ARGS__)
#endif