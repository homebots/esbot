#ifndef _TIMER_
#define _TIMER_

#include <Arduino.h>
#include <stdlib.h>
#include "./timer.h"

void Timer::start() {
  nextTick = micros() + interval;
  running = true;
}

void Timer::stop() {
  running = false;
}

void Timer::setInterval(unsigned long newInterval) {
  interval = newInterval;
}

void Timer::loop() {
  if (this->running == false) return;

  if (micros() > nextTick) {
    nextTick += interval;
    tick();
  }
}

#endif