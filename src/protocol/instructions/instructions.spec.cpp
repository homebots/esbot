// #define DEBUG(...) printf(__VA_ARGS__);

#include "assert.h"
#include "mock/arduino.mock.cpp"
#include "mock/ESP.mock.cpp"
#include "mock/serial.mock.cpp"
#include "../stream/main.cpp"
#include "./instructions.cpp"

int main() {
  beforeEach([] {
    resetArduinoMock();
    ESP.wasRestarted = false;
  });

  describe("parseInstruction", [] {
    it("should run instruction NOOP", [] {
      unsigned char stream[] = { BiNoop };
      Instructions i;
      i.parseInstruction(stream, 0);
    });

    it("should run instruction RESET", [] {
      unsigned char stream[] = { BiReset };
      Instructions i;
      i.parseInstruction(stream, 0);

      expect(ESP.wasRestarted).toBe(true);
    });

    it("should run instruction WRITE and turn pin 1 HIGH", [] {
      unsigned char stream[] = { BiWrite, 0x01, 0x01 };
      Instructions i;
      i.parseInstruction(stream, 0);

      expect(digitalRead(1)).toBe(true);
      expect(getPinMode(1)).toBe(OUTPUT);
    });

    it("should run instruction WRITE and turn pin 1 LOW", [] {
      unsigned char stream[] = { BiWrite, 0x01, 0x00 };
      digitalWrite(1, 1);

      Instructions i;
      i.parseInstruction(stream, 0);

      expect(digitalRead(1)).toBe(false);
      expect(getPinMode(1)).toBe(OUTPUT);
    });

    it("should read instruction ANALOG_WRITE and set pin 2 to 255", [] {
      unsigned char stream[] = { BiAnalogWrite, 0x02, '0', '0', 'f', 'f' };
      Instructions i;
      i.parseInstruction(stream, 0);

      expect(analogRead(2)).toBe(255);
      expect(getPinMode(2)).toBe(OUTPUT);
    });

    it("should run instruction READ and return value of pin 3", [] {
      unsigned char stream[] = { BiRead, 0x03, 0x00 };
      Instructions i;
      digitalWrite(3, 1);

      class ReadOutput: public InstructionLongResponse {
        public:
          int value;
          void call(unsigned char output) {
            this->value = output;
          }
      };

      ReadOutput reader;
      i.parseInstruction(stream, &reader);

      expect(reader.value).toBe(HIGH);
      expect(getPinMode(3)).toBe(INPUT);
    });

    xit("should read instruction READ and read analog value of pin 2", [] {
      unsigned char stream[] = { BiRead, 0x03, 0x01 };
      Instructions i;
      analogWrite(2, 128);

      class ReadOutput: public InstructionResponse {
        public:
          int value;
          void call(unsigned char* output) {
            this->value = output[0];
          }
      };

      ReadOutput reader;
      i.parseInstruction(stream, &reader);

      expect(reader.value).toBe(128);
      expect(getPinMode(2)).toBe(INPUT);
    });

    it("should read instruction DEBUG and start serial monitor", [] {
      unsigned char stream[] = { BiDebug, 0x01 };
      Instructions i;
      i.parseInstruction(stream, 0);

      expect(Serial.rate).toBe(115200);
    });

    it("should read instruction DEBUG and stop serial monitor", [] {
      unsigned char stream[] = { BiDebug, 0x00 };
      Serial.rate = 9600;

      Instructions i;
      i.parseInstruction(stream, 0);

      expect(Serial.rate).toBe(0);
    });
  });

  testSummary();
}