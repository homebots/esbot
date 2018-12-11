#ifndef _LOOPER_
#define _LOOPER_

class LoopCallable {
  public:
    virtual void loop(){}
};

struct Looper {
  int id;
  LoopCallable* fn;
  Looper* next;
  Looper* previous;
};

class Loop {
  Looper LOOP_TAIL;

  public:
    int length = 0;

    Loop() {
      LOOP_TAIL.previous = 0;
      LOOP_TAIL.next = 0;
      LOOP_TAIL.id = -1;
    }

    int add(LoopCallable* fn) {
      Looper* node = new Looper;
      node->previous = 0;
      node->next = 0;
      node->id = ++length;
      node->fn = fn;

      if (LOOP_TAIL.previous != 0) {
        node->previous = LOOP_TAIL.previous;
        node->previous->next = node;
      }

      node->next = &LOOP_TAIL;
      LOOP_TAIL.previous = node;

      return node->id;
    }

    void remove(int id) {
      Looper* node = &LOOP_TAIL;

      if (node->previous == 0) {
        return;
      }

      while (1) {
        if (node->id == id) {
          node->next->previous = node->previous;

          if (node->previous != 0) {
            node->previous->next = node->next;
          }

          length--;
          delete node;
          break;
        }

        if (node->previous == 0) {
          break;
        }

        node = node->previous;
      }
    }

    void loop() {
      Looper* node = &LOOP_TAIL;

      while(1) {
        if (node->previous == 0) {
          break;
        }

        node = node->previous;
        node->fn->loop();
      }
    }
};

Loop GlobalLoop;

#endif