// #define DEBUG(...) printf(__VA_ARGS__);

#include "assert.h"
#include "mock/arduino.mock.cpp"
#include "mock/ESP.mock.cpp"
#include "mock/serial.mock.cpp"
#include "mock/sockets.mock.cpp"
#include "protocol/stream/main.cpp"
#include "protocol/instructions.cpp"

int main() {
  beforeEach([] {
    resetArduinoMock();
    ESP.wasRestarted = false;
  });

  describe("parseInstruction", [] {
    it("should run instruction NOOP", [] {
      unsigned char stream[] = { BiNoop };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);
    });

    it("should run instruction RESET", [] {
      unsigned char stream[] = { BiReset };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);

      expect(ESP.wasRestarted).toBe(true);
    });

    it("should run instruction WRITE and turn pin 1 HIGH", [] {
      unsigned char stream[] = { BiWrite, 0x01, 0x01 };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);

      expect(digitalRead(1)).toBe(true);
      expect(getPinMode(1)).toBe(OUTPUT);
    });

    it("should run instruction WRITE and turn pin 1 LOW", [] {
      unsigned char stream[] = { BiWrite, 0x01, 0x00 };
      digitalWrite(1, 1);

      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);

      expect(digitalRead(1)).toBe(false);
      expect(getPinMode(1)).toBe(OUTPUT);
    });

    it("should read instruction ANALOG_WRITE and set pin 2 to 255", [] {
      unsigned char stream[] = { BiAnalogWrite, 0x02, '0', '0', 'f', 'f' };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);

      expect(analogRead(2)).toBe(255);
      expect(getPinMode(2)).toBe(OUTPUT);
    });

    it("should run instruction READ and return value of pin 3", [] {
      unsigned char stream[] = { BiRead, 0x03, 0x00 };
      unsigned char expectedResponse[] = { BiRead, 0x00, 0x01 };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      digitalWrite(3, 1);

      i.parseInstruction(stream);

      expect(strcmp((const char*)socketMock.lastBIN, (const char*)expectedResponse)).toBe(0);
      expect(getPinMode(3)).toBe(INPUT);
    });

    /*xit("should read instruction READ and read analog value of pin 2", [] {
      unsigned char stream[] = { BiRead, 0x03, 0x01 };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      analogWrite(2, 128);

      i.parseInstruction(stream);
      // (const char*)
      expect(strtoul(socketMock.lastBIN)).toBe(128);
      expect(getPinMode(2)).toBe(INPUT);
    });*/

    it("should read instruction DEBUG and start serial monitor", [] {
      unsigned char stream[] = { BiDebug, 0x01 };
      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);

      expect(Serial.rate).toBe(115200);
    });

    it("should read instruction DEBUG and stop serial monitor", [] {
      unsigned char stream[] = { BiDebug, 0x00 };
      Serial.rate = 9600;

      WebSocketsClient socketMock;
      Instructions i;
      i.setSocket(&socketMock);
      i.parseInstruction(stream);

      expect(Serial.rate).toBe(0);
    });
  });

  testSummary();
}