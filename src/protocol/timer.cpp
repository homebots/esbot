#include "../loop.cpp"

#ifndef _TIMER_
#define _TIMER_

#ifndef _ASSERT_CPP_
#include <Arduino.h>
#include <stdlib.h>
#include <functional>
#endif

class Timer: public LoopCallable {
  protected:
    int interval;
    int nextTick;
    bool running = false;
    std::function<void()> callback;

  public:
    Timer(int interval, std::function<void()> callback):
      interval(interval),
      callback(callback) {
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
        this->callback();
      }
    }
};

#endif