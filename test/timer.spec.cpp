// #define DEBUG(...) printf(__VA_ARGS__);

#include "mock/time.mock.cpp"
#include "assert.h"
#include "loop.cpp"
#include "timer.cpp"

int main() {
  describe("timer", []{
    it("should tick a callback every 3 milliseconds", []{
      int iterations = 0;
      std::function<void()> test = [&] {
        iterations++;
      };

      Timer timer(3, test);
      timer.start();

      tick(); Loop::loop();
      expect(iterations).toBe(0);
      expect(millis()).toBe(2);

      tick(); Loop::loop();
      tick(); Loop::loop();
      tick(); Loop::loop();
      expect(iterations).toBe(1);
      expect(millis()).toBe(5);

      tick(); Loop::loop();
      tick(); Loop::loop();
      tick(); Loop::loop();
      expect(iterations).toBe(2);
      expect(millis()).toBe(8);
    });
  });

  testSummary();
}