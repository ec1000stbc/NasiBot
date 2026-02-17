#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <SoftPWM.h>

class Led {
  private:
    int pin;
    bool state;
    bool useSoftPwm;
    unsigned long previousMillis;

  public:
    Led(int pin);

    void on();
    void off();
    void dimm(int val);
    void blink();
    void blink(int milliSeconds);
    void status();
    void toggle();
    bool isOn();
};

#endif
