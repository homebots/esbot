#include "assert.h"
#include "loop.cpp"

int main() {
  describe("Loop", []{
    beforeEach([]{
      Loop::global = new Loop;
    });

    afterEach([]{
      delete Loop::global;
    });

    describe("add and remove a callable object", []{
      LoopCallable noop;
      int id = Loop::global->add(&noop);

      expect(id).toBe(1);
      expect(Loop::global->length).toBe(1);

      Loop::global->remove(id);
      expect(Loop::global->length).toBe(0);
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

      int idA = Loop::global->add(&incA);
      int idB = Loop::global->add(&incB);

      expect(Loop::global->length).toBe(2);

      Loop::global->loop();
      expect(a).toBe(1);
      expect(b).toBe(2);

      Loop::global->loop();
      expect(a).toBe(2);
      expect(b).toBe(3);

      Loop::global->remove(idA);
      expect(Loop::global->length).toBe(1);

      Loop::global->loop();
      expect(a).toBe(2);
      expect(b).toBe(4);

      Loop::global->remove(idB);
      expect(Loop::global->length).toBe(0);
    });
  });

  testSummary();
}