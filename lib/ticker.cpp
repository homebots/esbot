#include <Arduino.h>
#include "./debug.h"

#ifndef _TICKER_
#define _TICKER_

typedef void (*TickerCallback)();

class Ticker {
  protected:
    int interval;
    int lastTick;
    int nextTick;
    bool running = false;
    TickerCallback tick = NULL;

  public:
    Ticker(int interval) {
      this->interval = interval;
    }

    void onTick(TickerCallback ticker) {
      this->tick = ticker;
    }

    void start() {
      this->lastTick = millis();
      this->nextTick = this->lastTick + this->interval;
      this->running = true;
    }

    void stop() {
      this->running = false;
    }

    void loop() {
      if (this->running == false) return;

      if (millis() > this->nextTick) {
        this->nextTick += this->interval;
        DEBUG.printf(">>> TICK\n");
        // this->tick();
      }
    }
};

#endif