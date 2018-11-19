#include <Arduino.h>
#include "./debug.h"
#include "./instruction.h"

int ANALOG_MAX = 200;

class PinController {
  private:
    uint8_t pin;

  public:
    PinController(uint8_t pin) {
      this->pin = pin;
    }

    void toOutput() {
      pinMode(this->pin, OUTPUT);
    }

    void toInput() {
      pinMode(this->pin, INPUT);
    }

    void on() {
      this->toOutput();
      digitalWrite(this->pin, HIGH);
    }

    void off() {
      this->toOutput();
      digitalWrite(this->pin, LOW);
    }

    bool isOn() {
      int state = digitalRead(this->pin);
      return state == HIGH;
    }

    bool isOff() {
      return !this->isOn();
    }

    void invert() {
      if (this->isOn()) {
        this->off();
        return;
      }

      this->on();
    }

    void blink(uint8_t times, uint8_t interval) {
      while (times > 0) {
        this->invert();
        delay(interval);

        this->invert();
        delay(interval);

        times--;
      }
    }

    void blinkFade(uint8_t times, uint8_t msDelay) {
      int brightness = 1;
      int amount = 5;

      if (times <= 0) {
        return;
      }

      while (times > 0) {
        analogWrite(this->pin, brightness);
        brightness = brightness + amount;

        if (brightness <= 1 || brightness >= ANALOG_MAX) {
          amount = amount * -1;
        }

        if (brightness >= ANALOG_MAX) {
          brightness = ANALOG_MAX;
        }

        if (brightness <= 1) {
          times--;
          brightness = 1;
        }

        delay(msDelay);
      }
    }

    int read() {
      this->toInput();
      return digitalRead(this->pin);
    }

    int readAnalog() {
      this->toInput();
      return analogRead(this->pin);
    }

    void writeAnalog(uint8_t value) {
      this->toOutput();
      analogWrite(this->pin, value);
    }
};
