#include "Led_Basic.h"

Led::Led(int p) {
  pin = p;
  state = false;
  pinMode(pin, OUTPUT);
}

void Led::on() {
  state = true;
  digitalWrite(pin, HIGH);
}

void Led::off() {
  state = false;
  digitalWrite(pin, LOW);
}

bool Led::isOn() {
  return state;
}

void Led::toggle() {
  state = !state;
  digitalWrite(pin, state ? HIGH : LOW);
}