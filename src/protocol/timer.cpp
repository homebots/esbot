#include <Arduino.h>
#include "./debug.h"

#ifndef _TICKER_
#define _TICKER_

typedef void (*TickerCallback)();

class Timer {
  protected:
    int _interval;
    TickerCallback _onTick = NULL;

  public:
    int nextTick;
    bool running = false;

    Timer(int interval): _interval(interval) {}

    void onTick(TickerCallback onTick) {
      _onTick = onTick;
    }

    void start() {
      nextTick = millis() + _interval;
      running = true;
    }

    void stop() {
      running = false;
    }

    void setInterval(int interval) {
      _interval = interval;
    }

    void loop() {
      if (this->running == false) return;

      if (millis() > nextTick) {
        nextTick += _interval;
        this->_onTick();
      }
    }
};

#endif