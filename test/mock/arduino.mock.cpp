unsigned char LED_BUILTIN = 5;
unsigned char LOW = 0x00;
unsigned char HIGH = 0x01;
unsigned char INPUT = 0x01;
unsigned char OUTPUT = 0x02;

// GPIO 0, 1, 2, 3, LED_BUILTIN
unsigned char digitalPins[5] = {0, 0, 0, 0, 0};
unsigned long analogPins[5] = {0, 0, 0, 0, 0};

// 1 = input, 2 = output, 0 = undefined
unsigned long pinModes[5] = {0, 0, 0, 0, 0};

void digitalWrite(unsigned char pin, unsigned char level) {
  digitalPins[pin] = level;
}

unsigned char digitalRead(unsigned char pin) {
  return digitalPins[pin];
}

void analogWrite(unsigned char pin, unsigned long level) {
  analogPins[pin] = level;
}

unsigned long analogRead(unsigned char pin) {
  return analogPins[pin];
}

void pinMode(unsigned char pin, unsigned char mode) {
  pinModes[pin] = mode;
}

unsigned char getPinMode(unsigned char pin) {
  return pinModes[pin];
}

void resetArduinoMock() {
  for (char i = 0; i < 5; i++) {
    digitalPins[i] = 0;
    analogPins[i] = 0;
    pinModes[i] = 0;
  }
}