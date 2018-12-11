#ifndef _LOOP_H_
#define _LOOP_H_

class LoopCallable {
  public:
    virtual void loop();
};

struct Looper {
  int id;
  LoopCallable* fn;
  Looper* next;
  Looper* previous;
};

class Loop {
  public:
    Looper LOOP_TAIL;
    int length = 0;

    static Loop* global;

    Loop();
    int add(LoopCallable* fn);
    void remove(int id);
    void loop();
};

#endif