#include "loop.h"
#include "callable.h"

#ifndef _TIMER_
#define _TIMER_

class Timer: public LoopCallable {
  protected:
    int interval;
    int nextTick;
    bool running = false;
    Callable* callable;

  public:
    Timer(int interval, Callable* callback):
      interval(interval),
      callable(callback) {
        GlobalLoop.add(this);
      }

    void start() {
      nextTick = millis() + interval;
      running = true;
    }

    void stop() {
      running = false;
    }

    void setInterval(int newInterval) {
      interval = newInterval;
    }

    void loop() {
      if (this->running == false) return;

      if (millis() > nextTick) {
        nextTick += interval;
        this->callable->call();
      }
    }
};

#endif