// #define DEBUG(...) printf(__VA_ARGS__);

#include <string.h>
#include "assert.h"
#include "./stream-writer.cpp"

int main() {
  describe("StreamWriter", []{
    it("should store booleans", []{
      StreamWriter writer;

      writer.writeBool(true);
      writer.writeBool(false);

      unsigned char* output = writer.getStream();
      expect(output[0] == 0x30).toBe(true);
      expect(output[1] == 0x31).toBe(true);
      expect(output[2] == 0x00).toBe(true);
    });

    it("should store small numbers (0..255)", []{
      StreamWriter writer;
      const char* bytes = "42";

      writer.writeByte(bytes[0]);
      writer.writeByte(bytes[1]);
      writer.writeByte(0);
      writer.writeByte(1);
      writer.writeByte(255);

      unsigned char* output = writer.getStream();
      expect(output[0] == bytes[0]).toBe(true);
      expect(output[1] == bytes[1]).toBe(true);
      expect(output[2] == 0).toBe(true);
      expect(output[3] == 1).toBe(true);
      expect(output[4] == 255).toBe(true);
      expect(output[5] == 0x00).toBe(true);
    });

    it("should store big numbers (0..65535)", []{
      StreamWriter writer;
      const char* expected = "000000ff0400ffff";

      writer.writeNumber(0);
      writer.writeNumber(255);
      writer.writeNumber(1024);
      writer.writeNumber(65535);

      unsigned char* output = writer.getStream();
      expect(output).toBe(expected);
    });

    it("should store strings", []{
      StreamWriter writer;
      const char* expected = "dead""beef";

      writer.writeString("dead");
      writer.writeString("beef");

      unsigned char* output = writer.getStream();
      expect(output).toBe(expected);
    });

    it("should reset after returning byte stream", []{
      StreamWriter writer;
      const char* dead = "dead";
      const char* beef = "beef";

      writer.writeString(dead);

      unsigned char* outputOne = writer.getStream();
      expect(outputOne).toBe(dead);

      writer.writeString(beef);

      unsigned char* outputTwo = writer.getStream();
      expect(outputTwo).toBe(beef);
    });
  });

  testSummary();
}