#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <SoftPWM.h>

/* ================= LED ENUM ================= */

enum LedId {
  LED_FRONT_LEFT,
  LED_FRONT_RIGHT,
  LED_TOP_LEFT,
  LED_TOP_RIGHT,
  LED_FRONT        // logische Gruppe
};

class Led {
  private:
    uint8_t pin1;
    uint8_t pin2;        // nur genutzt bei Gruppen (z.B. FRONT)
    bool isGroup;

    bool state;
    bool useSoftPwm;
    unsigned long previousMillis;

    void initPin(uint8_t pin);
    bool isHardwarePwmPin(uint8_t pin);

  public:
    Led(LedId id);

    void on();
    void off();
    void dimm(int val);
    void blink();
    void blink(int milliSeconds);
    void toggle();
    bool isOn();
};

#endif
