// #define DEBUG(...) printf(__VA_ARGS__);

#include "assert.h"
#include "./stream-reader.cpp"

int main() {
  describe("StreamReader", [] {
    it("should read strings", [] {
      StreamReader reader((unsigned char*)"abcd\0ff\0");
      expect(reader.readString()).toBe("abcd");
      expect(reader.readString()).toBe("ff");
    });

    it("should read a boolean (0..1)", [] {
      unsigned char stream[] = { 0x00, 0x01, 0x02 };
      StreamReader reader(stream);

      expect(reader.readBool()).toBe(false);
      expect(reader.readBool()).toBe(true);
      expect(reader.readBool()).toBe(false);
    });

    it("should read a sequence of small numbers (0..255)", [] {
      unsigned char stream[] = { 0x01, 0x7f, 0xff };
      StreamReader reader(stream);

      expect(reader.readByte()).toBe(0x01);
      expect(reader.readByte()).toBe(0x7f);
      expect(reader.readByte()).toBe(0xff);
    });

    it("should read long numbers 4 bytes at time", [] {
      StreamReader reader((unsigned char*)"00040000ffff");

      expect(reader.readLong()).toBe(4);
      expect(reader.readLong()).toBe(0);
      expect(reader.readLong()).toBe(65535);
    });

    it("should read different data types in a stream", [] {
      unsigned char stream[] = "abz\0" "\x0f" "\xff" "0400ffff" "\x01";
      StreamReader reader(stream);
      expect(reader.readString()).toBe("abz");
      expect(reader.readByte()).toBe(0x0f);
      expect(reader.readByte()).toBe(0xff);
      expect(reader.readLong()).toBe(1024);
      expect(reader.readLong()).toBe(65535);
      expect(reader.readBool()).toBe(true);
    });
  });

  testSummary();
}