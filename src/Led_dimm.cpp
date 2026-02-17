#include "Led_Dimm.h"

Led::Led(int pin)
  : pin(pin), state(false), previousMillis(0) {

  if (pin == 3 || pin == 5 || pin == 6 ||
      pin == 9 || pin == 10 || pin == 11) {

    useSoftPwm = false;
    pinMode(pin, OUTPUT);
    Serial.println("Digital");

  } else {
    useSoftPwm = true;
    SoftPWMSet(pin, 0);
    SoftPWMSetFadeTime(pin, 0, 0);
  }
}

void Led::on() {
  state = true;
  if (useSoftPwm)
    SoftPWMSet(pin, 255);
  else
    analogWrite(pin, 255);
}

void Led::off() {
  state = false;
  if (useSoftPwm)
    SoftPWMSet(pin, 0);
  else
    digitalWrite(pin, LOW);
}

void Led::dimm(int val) {
  if (val > 0) {
    state = true;
    if (useSoftPwm)
      SoftPWMSet(pin, val);
    else
      analogWrite(pin, val);
  } else {
    off();
  }
}

void Led::blink() {
  blink(500);
}

void Led::blink(int milliSeconds) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= milliSeconds) {
    state = !state;
    digitalWrite(pin, state ? HIGH : LOW);
    previousMillis = currentMillis;
  }
}

void Led::status() {
  Serial.print(state);
}
