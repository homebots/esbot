//#define DEBUG(...) printf(__VA_ARGS__);

#include "./assert.h"
#include "../lib/stream-reader.cpp"

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

      expect(reader.readByte()).toBe(1);
      expect(reader.readByte()).toBe(127);
      expect(reader.readByte()).toBe(255);
    });

    it("should read numbers 3 bytes at time (0..4095)", [] {
      StreamReader reader((unsigned char*)"00040000ffff");

      expect(reader.readNumber()).toBe(0);
      expect(reader.readNumber()).toBe(1024);
      expect(reader.readNumber()).toBe(15);
      expect(reader.readNumber()).toBe(4095);
    });

    it("should read long numbers 4 bytes at time", [] {
      StreamReader reader((unsigned char*)"00040000ffff");

      expect(reader.readLong()).toBe(4);
      expect(reader.readLong()).toBe(0);
      expect(reader.readLong()).toBe(65535);
    });

    it("should read different data types in a stream", [] {
      // unsigned char stream[] = { 'a', 'b', 'z', 0x00, 0x0f, 0xff, '4', '0', '0', 'f', 'f', 'f', 'f', 0x01 };
      unsigned char stream[] = "abz\0" "\x0f" "\xff" "400ffff" "\x01";
      StreamReader reader(stream);
      expect(reader.readString()).toBe("abz");
      expect(reader.readByte()).toBe(15);
      expect(reader.readByte()).toBe(255);
      expect(reader.readNumber()).toBe(1024);
      expect(reader.readLong()).toBe(65535);
      expect(reader.readBool()).toBe(true);
    });
  });
}