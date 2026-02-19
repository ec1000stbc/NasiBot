#include "IR_Sensor.h"

IRSensor::IRSensor(int pin) : pin(pin) {}

void IRSensor::begin() {
  pinMode(pin, INPUT);
}

bool IRSensor::detected() {
  // Die meisten IR-Line-Sensoren:
  // LOW  = Linie (schwarz)
  // HIGH = kein Linie (weiß)
  return digitalRead(pin) == LOW;
}

int IRSensor::readRaw() {
  return digitalRead(pin);
}

int IRSensor::getPin() {
  return pin;
}
