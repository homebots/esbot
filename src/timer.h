#ifndef _TIMER_H_
#define _TIMER_H_

class Timer {
  protected:
    int interval;
    int nextTick;
    bool running = false;

  public:
    void start();
    void stop();
    void setInterval(unsigned long newInterval);
    void loop();
    virtual void tick();
};

#endif