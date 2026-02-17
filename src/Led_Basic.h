#ifndef LED_BASIC_H
#define LED_BASIC_H

#include <Arduino.h>

class Led {
  private:
    int pin;
    bool state;

  public:
    Led(int pin);
    void on();
    void off();
    bool isOn();
    void toggle();
};

#endif