#include "Led.h"

/* ================= PIN-MAPPING ================= */

static uint8_t ledPinFromId(LedId id, uint8_t &pin2, bool &isGroup) {
  isGroup = false;
  pin2 = 255;

  switch (id) {
    case LED_FRONT_LEFT:  return A2;
    case LED_FRONT_RIGHT: return A3;
    case LED_TOP_LEFT:    return 9;
    case LED_TOP_RIGHT:   return 10;

    case LED_FRONT:
      isGroup = true;
      pin2 = A3;
      return A2;
  }
  return 255;
}

/* ================= KONSTRUKTOR ================= */

Led::Led(LedId id)
  : state(false), previousMillis(0) {

  pin1 = ledPinFromId(id, pin2, isGroup);

  initPin(pin1);
  if (isGroup) initPin(pin2);
}

/* ================= INIT ================= */

void Led::initPin(uint8_t pin) {
  if (isHardwarePwmPin(pin)) {
    pinMode(pin, OUTPUT);
    useSoftPwm = false;
  } else {
    SoftPWMSet(pin, 0);
    SoftPWMSetFadeTime(pin, 0, 0);
    useSoftPwm = true;
  }
}

bool Led::isHardwarePwmPin(uint8_t pin) {
  return (pin == 3 || pin == 5 || pin == 6 ||
          pin == 9 || pin == 10 || pin == 11);
}

/* ================= BASIS ================= */

void Led::on() {
  state = true;

  if (useSoftPwm) {
    SoftPWMSet(pin1, 255);
    if (isGroup) SoftPWMSet(pin2, 255);
  } else {
    analogWrite(pin1, 255);
    if (isGroup) analogWrite(pin2, 255);
  }
}

void Led::off() {
  state = false;

  if (useSoftPwm) {
    SoftPWMSet(pin1, 0);
    if (isGroup) SoftPWMSet(pin2, 0);
  } else {
    digitalWrite(pin1, LOW);
    if (isGroup) digitalWrite(pin2, LOW);
  }
}

void Led::dimm(int val) {
  val = constrain(val, 0, 255);

  if (val == 0) {
    off();
    return;
  }

  state = true;

  if (useSoftPwm) {
    SoftPWMSet(pin1, val);
    if (isGroup) SoftPWMSet(pin2, val);
  } else {
    analogWrite(pin1, val);
    if (isGroup) analogWrite(pin2, val);
  }
}

void Led::toggle() {
  state ? off() : on();
}

bool Led::isOn() {
  return state;
}

/* ================= BLINK ================= */

void Led::blink() {
  blink(500);
}

void Led::blink(int milliSeconds) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= (unsigned long)milliSeconds) {
    toggle();
    previousMillis = currentMillis;
  }
}
