#ifndef _TIMER_
#define _TIMER_

#include <Arduino.h>
#include <stdlib.h>
#include "./loop.h"

class Timer: public LoopCallable {
  protected:
    int interval;
    int nextTick;
    bool running = false;
    void (*callback)();

  public:
    Timer(int interval, void (*cb)()):
      interval(interval),
      callback(cb) {}

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
        this->callback();
      }
    }
};

#endif