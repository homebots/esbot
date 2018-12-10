// #define DEBUG(...) printf(__VA_ARGS__)
#include "assert.h"
#include "./loop.cpp"

int main() {
  describe("GlobalLoop", []{
    describe("add and remove a callable object", []{
      LoopCallable noop;
      int id = GlobalLoop.add(&noop);

      expect(id).toBe(1);
      expect(GlobalLoop.length).toBe(1);

      GlobalLoop.remove(id);
      expect(GlobalLoop.length).toBe(0);
    });

    describe("call stored loop objects", []{
      int a = 0;
      int b = 1;

      class Increment: public LoopCallable {
        int* i;

        public:
          Increment(int* number): i(number) {}
          void loop() { *i = *i + 1; }
      };

      Increment incA(&a);
      Increment incB(&b);

      int idA = GlobalLoop.add(&incA);
      int idB = GlobalLoop.add(&incB);

      expect(GlobalLoop.length).toBe(2);

      GlobalLoop.loop();
      expect(a).toBe(1);
      expect(b).toBe(2);

      GlobalLoop.loop();
      expect(a).toBe(2);
      expect(b).toBe(3);

      GlobalLoop.remove(idA);
      expect(GlobalLoop.length).toBe(1);

      GlobalLoop.loop();
      expect(a).toBe(2);
      expect(b).toBe(4);

      GlobalLoop.remove(idB);
      expect(GlobalLoop.length).toBe(0);
    });
  });

  testSummary();
}