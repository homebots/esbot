//#define DEBUG(...) printf(__VA_ARGS__);

#include "./assert.h"
#include "./arduino.mock.cpp"
#include "./Esp.mock.cpp"
#include "../lib/bot.cpp"

int main() {
  beforeEach([] {
    resetArduinoMock();
    Esp.wasRestarted = false;
  });

  describe("parseInstruction", [] {
    it("should run instruction NOOP", [] {
      unsigned char stream[] = { BiNoop };
      Instruction* i = parseInstruction(stream);
      i->run();
      expect(i->id).toBe(BiNoop);
    });

    it("should run instruction RESET", [] {
      unsigned char stream[] = { BiReset };
      Instruction* i = parseInstruction(stream);
      i->run();

      expect(ESP.wasRestarted).toBe(true);
      expect(i->id).toBe(BiReset);
    });

    it("should run instruction WRITE and turn pin 1 HIGH", [] {
      unsigned char stream[] = { BiWrite, 0x01, 0x01 };
      Instruction* i = parseInstruction(stream);
      i->run();

      expect(digitalRead(1)).toBe(true);
      expect(getPinMode(1)).toBe(OUTPUT);
      expect(i->id).toBe(BiWrite);
    });

    it("should run instruction WRITE and turn pin 1 LOW", [] {
      unsigned char stream[] = { BiWrite, 0x01, 0x00 };
      Instruction* i = parseInstruction(stream);
      digitalWrite(1, 1);
      i->run();

      expect(digitalRead(1)).toBe(false);
      expect(getPinMode(1)).toBe(OUTPUT);
    });

    it("should run instruction READ and return value of pin 3", [] {
      unsigned char stream[] = { BiRead, 0x03 };
      Instruction* i = parseInstruction(stream);
      digitalWrite(3, 1);
      i->run();

      expect(i->digitalOutput).toBe(HIGH);
      expect(getPinMode(3)).toBe(INPUT);
    });

    it("should read instruction ANALOG_WRITE and set pin 1 to 255", [] {
      unsigned char stream[] = { BiAnalogWrite, 0x02, '0', 'f', 'f' };
      Instruction* i = parseInstruction(stream);
      i->run();

      expect(analogRead(2)).toBe(255);
      expect(getPinMode(2)).toBe(OUTPUT);
    });

    it("should read instruction ANALOG_READ and read value of pin 2", [] {
      unsigned char stream[] = { BiAnalogRead, 0x02 };
      analogWrite(2, 128);
      Instruction* i = parseInstruction(stream);
      i->run();

      expect(i->analogOutput).toBe(128);
      expect(getPinMode(2)).toBe(INPUT);
    });
  });

  testSummary();
}